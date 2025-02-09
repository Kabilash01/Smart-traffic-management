
# **Smart Traffic Management System 🚦🚗**
This project implements a **multi-threaded Smart Traffic Management System** with **Priority-based Scheduling, Round Robin, and Real-time Visualization**.

## **🛠 Features**
✅ **Multi-threading using POSIX Threads**  
✅ **Priority-based Scheduling & Round Robin**  
✅ **GTK-based GUI with Cairo Rendering**  
✅ **Flask Web Server for Live Traffic Monitoring**  
✅ **Socket-based Communication for Real-time Updates**  
✅ **Adaptive Traffic Control for Congestion Management**  

---

## **📌 Prerequisites**
Ensure the following dependencies are installed:

### **🔹 Install Required Packages**
```bash
sudo apt update
sudo apt install -y gcc make libgtk-3-dev python3 python3-pip python3-venv
```
### **🔹 Install Flask (For Web Server)**
```bash
python3 -m venv myvenv
source myvenv/bin/activate
pip install flask
```

---

## **🚀 How to Run the Project**
### **1️⃣ Clone the Repository**
```bash
git clone https://github.com/yourusername/smart-traffic-management.git
cd smart-traffic-management
```

### **2️⃣ Start Flask Web Server**
```bash
source myvenv/bin/activate
python3 server.py
```
- The Flask server will run on `http://127.0.0.1:5001/`

---

### **3️⃣ Compile & Run the C Program**
```bash
gcc smart.c -o traffic_simulator `pkg-config --cflags --libs gtk+-3.0` -lpthread -lm
./traffic_simulator
```
- This **starts the traffic simulation** with **real-time visualization**.

---

### **4️⃣ Open the Web Interface**
After running the Flask server and C program, open the browser and visit:
```
http://127.0.0.1:5001
```
Here, you can **view real-time vehicle updates and traffic conditions**.

---

## **📊 System Components**
### **1️⃣ C Program (`smart_traffic_scheduler.c`)**
- Implements **multi-threading** for vehicle movement.
- Uses **GTK & Cairo** for a graphical interface.
- Sends **live traffic data to Flask** via **sockets**.

### **2️⃣ Web Server (`server.py`)**
- Built with **Flask** to handle incoming vehicle data.
- Serves **real-time vehicle updates** to the web dashboard.

### **3️⃣ Web Interface (`index.html`)**
- Displays **live vehicle updates** with color-coded traffic conditions.
- Uses **CSS for styling** and **JavaScript for real-time updates**.

---

## **🌍 Publishing to GitHub**
To **push this project to GitHub**, follow these steps:

### **1️⃣ Initialize Git**
```bash
git init
git add .
git commit -m "Initial commit - Smart Traffic Management System"
```

### **2️⃣ Link GitHub Repository**
```bash
git remote add origin https://github.com/yourusername/smart-traffic-management.git
git branch -M main
git push -u origin main
```

---

## **📌 Additional Commands**
### **🔹 Check Flask Server Logs**
```bash
tail -f server.log
```
### **🔹 Test API Response**
```bash
curl -X GET http://127.0.0.1:5001/traffic
```

---

## **📄 License**
This project is licensed under the **MIT License**.

---

## **👨‍💻 Author**
**Your Name**  
GitHub: [@yourusername](https://github.com/kabilash01)  

---

## **🚦 Now Your Smart Traffic System is Ready!**
Enjoy **real-time traffic simulation, scheduling, and web-based monitoring!** 🚗🔥

