from flask import Flask, request, jsonify, render_template

app = Flask(__name__)
traffic_data = []  # Store vehicle updates

@app.route('/')
def index():
    return render_template("index.html")

@app.route('/update', methods=['POST'])
def update_traffic():
    data = request.get_json()
    if data:
        print(f"Received: {data}")  # Debugging log
        traffic_data.append(data)
    return jsonify({"status": "success"}), 200

@app.route('/traffic')
def get_traffic():
    return jsonify({"vehicles": traffic_data[-10:]})  # Show last 10 updates

if __name__ == "__main__":
    app.run(debug=True, port=5001)
