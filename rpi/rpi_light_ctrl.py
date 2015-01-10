#!/usr/bin/env python

from time import sleep
from flask import Flask, render_template
import threading
from light import Light

app = Flask(__name__)
light = Light()

@app.route("/light/")
def light_index():
    if light.getStatus():
        status = "The light is ON"
    else:
        status = "The light is OFF"

    return render_template('index.html', status=status)

@app.route("/light/<state>")
def turn_lgith(state):
    status = ''

    if state == 'on' or state == 'off':
        if light.turn(state == 'on'):
            status = "Success"
        else:
            status = "No response"
    else:
        satus = "Unknown argument."

    return render_template('index.html', status=status)

if __name__ == "__main__":
    app.run(host='0.0.0.0')
