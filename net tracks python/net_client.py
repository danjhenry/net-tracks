import requests
from win32 import win32api
import time

url = 'http://192.168.8.108/controls'

def keyState(keys):
    stack = []
    for key in keys:
        state = win32api.GetAsyncKeyState(ord(key))
        if(state < 0 or state == 1):
            stack.append(key)
    return stack

def main():
    state = False
    payload = {}
    while True:
        keysOn = keyState('WASD')
        if(keysOn):
            state = True
            payload['left_bank'] = 2
            payload['right_bank'] = 2
            for key in keysOn:
                if(key == 'W'):
                    payload['left_bank'] = 1
                    payload['right_bank'] = 1
                if(key == 'S'):
                    payload['left_bank'] = 0
                    payload['right_bank'] = 0
                if(key == 'A'):
                    payload['left_bank'] = 1
                    payload['right_bank'] = 0
                if(key == 'D'):
                    payload['left_bank'] = 0
                    payload['right_bank'] = 1
            r = requests.get(url, params=payload)
            print(r.text, '\n')
        elif state:
            state = False
            payload['left_bank'] = 2
            payload['right_bank'] = 2
            r = requests.get(url, params=payload)
            print(r.text, '\n')
main()
