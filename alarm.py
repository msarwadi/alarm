from flask import Flask, render_template, request
import pyfirmata
import serial
import time
from datetime import datetime

app = Flask(__name__)

port = '/dev/tty.usbmodem11101'

turn_on_time = "21:17:00"

def setup_board(port):
    board = pyfirmata.Arduino(port)
    return board

def turn_on_component(board, pin):
    board.digital[pin].write(1)

def main():
    board = setup_board(port)
    turn_on_hour, turn_on_minute, turn_on_second = map(int, turn_on_time.split(':'))

    while True:
        now = datetime.now()
        if (now.hour == turn_on_hour and now.minute == turn_on_minute and now.second == turn_on_second):
            turn_on_component(board, 3)
        
        time.sleep(1)

@app.route('/', methods=['GET', 'POST'])
def index():
    if request.method == 'POST':
        new_turn_on_time = request.form['turn_on_time']
        global turn_on_time
        turn_on_time = new_turn_on_time
    return render_template('index.html', turn_on_time=turn_on_time)

if __name__ == "__main__":
    import threading
    main_thread = threading.Thread(target=main)
    main_thread.start()

    app.run(host="0.0.0.0", port=5000, debug=True)