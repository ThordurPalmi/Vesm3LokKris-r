from flask import Flask, render_template, request, redirect, url_for
from datetime import datetime
import pyrebase
import requests
import time

ip = "192.168.1.140"

app = Flask(__name__)

gogn = []

firebase_config = {'apiKey': 'AIzaSyCbCFNzTXUrVG8IpuwDnq1XIl4OkZUaEIU',
 'authDomain': 'vesm-lokaverk.firebaseapp.com',
 'databaseURL': 'https://vesm-lokaverk-default-rtdb.europe-west1.firebasedatabase.app/',
 'projectId': 'vesm-lokaverk',
 'storageBucket': 'vesm-lokaverk.appspot.com',
 'messagingSenderId': '823778249927',
 'appId': '1:823778249927:web:d571f619b71e42f74202e2'}


firebase = pyrebase.initialize_app(firebase_config)
db = firebase.database()

@app.route('/')
def dash():
    global gogn
    
    url = 'https://vesm-lokaverk-default-rtdb.europe-west1.firebasedatabase.app/root/data/.json?orderBy="\\"{}\\""&limitToLast=20'.format("timestamp")  # sækja gögn úr gangna grunni með request því .orderBy() er bugy í pyrebase
    headers = {'header': 'application/json'}

    response = requests.get(url, headers=headers)

    gogn = []

    for entry in response.json():
        gogn.append(response.json()[entry])

    return render_template("index.html", gogn=gogn)



@app.route('/api', methods = ["GET","POST"])
def api():
    global gogn
    if request.method == "POST":
        data = request.get_json()


        now = datetime.now()

        timestamp = now.strftime("%Y-%m-%d %H:%M:%S")



        data["timestamp"] = timestamp
        print(data)


        db.child("root").child("data").push(data)
        time.sleep(8)
        return redirect(url_for("dash"))

@app.route('/refresh')
def refresh():
    url = "http://"+ip+":5050/"

    response = requests.post(url, data="refresh")

    if response.status_code == 200:
        print("POST request successful!")
    else:
        print("Error sending POST request")

    return redirect(url_for('dash'))
if __name__ == '__main__':
    app.run(debug=True, use_reloader=True)