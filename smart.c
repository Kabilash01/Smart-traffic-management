#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <cairo.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define NUM_LANES 4
#define ROAD_LENGTH 700
#define TIME_SLICE 1
#define AMBULANCE_PRIORITY 10
#define VIP_PRIORITY 8
#define HEAVY_PRIORITY 6
#define CAR_PRIORITY 4
#define BIKE_PRIORITY 2
#define PEDESTRIAN_PRIORITY 1
#define VEHICLE_COUNT (NUM_LANES * 5)

typedef struct {
    int id;
    int priority;
    char type[20];
    int position;
    int lane;
    char status[20];
    char color[20];
} Vehicle;

pthread_mutex_t lock;
Vehicle vehicles[VEHICLE_COUNT];
GtkWidget *drawing_area, *info_label;
int traffic_light_state[NUM_LANES] = {1, 1, 1, 1};
int total_cleared = 0;

void send_vehicle_data(const char *data) {
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) return;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5001);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        close(sockfd);
        return;
    }

    char request[1024];
    snprintf(request, sizeof(request),
             "POST /update HTTP/1.1\r\n"
             "Host: 127.0.0.1:5001\r\n"
             "Content-Type: application/json\r\n"
             "Content-Length: %ld\r\n\r\n"
             "%s", strlen(data), data);

    send(sockfd, request, strlen(request), 0);
    close(sockfd);
}

void generate_traffic() {
    srand(time(NULL));
    for (int i = 0; i < VEHICLE_COUNT; i++) {
        vehicles[i].id = i;
        vehicles[i].position = rand() % 200;
        vehicles[i].lane = i % NUM_LANES;
        strcpy(vehicles[i].status, "Moving");

        switch (rand() % 6) {
            case 0: strcpy(vehicles[i].type, "Ambulance"); vehicles[i].priority = AMBULANCE_PRIORITY; strcpy(vehicles[i].color, "Red"); break;
            case 1: strcpy(vehicles[i].type, "VIP"); vehicles[i].priority = VIP_PRIORITY; strcpy(vehicles[i].color, "Orange"); break;
            case 2: strcpy(vehicles[i].type, "Heavy Vehicle"); vehicles[i].priority = HEAVY_PRIORITY; strcpy(vehicles[i].color, "Yellow"); break;
            case 3: strcpy(vehicles[i].type, "Car"); vehicles[i].priority = CAR_PRIORITY; strcpy(vehicles[i].color, "Blue"); break;
            case 4: strcpy(vehicles[i].type, "Bike"); vehicles[i].priority = BIKE_PRIORITY; strcpy(vehicles[i].color, "Green"); break;
            case 5: strcpy(vehicles[i].type, "Pedestrian"); vehicles[i].priority = PEDESTRIAN_PRIORITY; strcpy(vehicles[i].color, "White"); break;
        }
    }
}

void* move_vehicles_thread(void *arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        char buffer[1024];

        for (int i = 0; i < VEHICLE_COUNT; i++) {
            if (traffic_light_state[vehicles[i].lane]) {
                int speed = (vehicles[i].priority / 2) + (rand() % 5);
                vehicles[i].position += speed;
                if (vehicles[i].position > ROAD_LENGTH) vehicles[i].position = 0;

                snprintf(buffer, sizeof(buffer), "{\"type\": \"%s\", \"priority\": %d, \"color\": \"%s\", \"lane\": %d, \"position\": %d}",
                         vehicles[i].type, vehicles[i].priority, vehicles[i].color, vehicles[i].lane, vehicles[i].position);
                send_vehicle_data(buffer);
            }
        }
        pthread_mutex_unlock(&lock);
        gtk_widget_queue_draw(drawing_area);
        usleep(100000); // Sleep for 100ms to prevent excessive CPU usage
    }
    return NULL;
}

void start_vehicle_thread() {
    pthread_t vehicle_thread;
    pthread_create(&vehicle_thread, NULL, move_vehicles_thread, NULL);
    pthread_detach(vehicle_thread);
}

gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data) {
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);

    for (int i = 0; i < NUM_LANES; i++) {
        cairo_set_source_rgb(cr, 0.2, 0.2, 0.2);
        cairo_rectangle(cr, 0, i * 100 + 20, ROAD_LENGTH, 80);
        cairo_fill(cr);
    }

    for (int i = 0; i < VEHICLE_COUNT; i++) {
        if (strcmp(vehicles[i].color, "Red") == 0) cairo_set_source_rgb(cr, 1, 0, 0);
        else if (strcmp(vehicles[i].color, "Blue") == 0) cairo_set_source_rgb(cr, 0, 0, 1);
        else if (strcmp(vehicles[i].color, "Green") == 0) cairo_set_source_rgb(cr, 0, 1, 0);
        else if (strcmp(vehicles[i].color, "Yellow") == 0) cairo_set_source_rgb(cr, 1, 1, 0);
        else if (strcmp(vehicles[i].color, "Orange") == 0) cairo_set_source_rgb(cr, 1, 0.5, 0);
        else cairo_set_source_rgb(cr, 1, 1, 1);

        cairo_rectangle(cr, vehicles[i].position, vehicles[i].lane * 100 + 40, 30, 30);
        cairo_fill(cr);
    }

    return FALSE;
}

int main(int argc, char *argv[]) {
    pthread_mutex_init(&lock, NULL);
    GtkWidget *window, *vbox;
    
    gtk_init(&argc, &argv);
    generate_traffic();

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Thread Scheduling Traffic System");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), ROAD_LENGTH, 500);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    info_label = gtk_label_new("Vehicle Info");
    gtk_box_pack_start(GTK_BOX(vbox), info_label, FALSE, FALSE, 0);

    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, ROAD_LENGTH, 400);
    g_signal_connect(drawing_area, "draw", G_CALLBACK(draw_callback), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 0);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    start_vehicle_thread();

    gtk_widget_show_all(window);
    gtk_main();

    pthread_mutex_destroy(&lock);
    return 0;
}
