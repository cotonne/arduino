#!/usr/bin/python
import time

import Adafruit_CharLCD as LCD
import RPi.GPIO as GPIO

import json
import twitter

def chunkstring(string, length):
    return (string[0+i:length+i] for i in range(0, len(string), length))

#########!!!!!! REPLACE WITH YOUR OWN INFORMATIONS !!!!
CONSUMER_KEY = 'CONSUMER_KEY'
CONSUMER_SECRET = 'CONSUMER_SECRET'
ACCESS_TOKEN = 'ACCESS_TOKEN'
ACCESS_TOKEN_SECRET = 'ACCESS_TOKEN_SECRET'

# Users to watch for should be a list. This will be joined by Twitter and the
# data returned will be for any tweet mentioning:
# @twitter *OR* @twitterapi *OR* @support.
# USERS = ['@twitter',
#         '@twitterapi',
#         '@support']
USERS = ['@twitter']

# Since we're going to be using a streaming endpoint, there is no need to worry
# about rate limits.
api = twitter.Api(CONSUMER_KEY,
          CONSUMER_SECRET,
          ACCESS_TOKEN,
          ACCESS_TOKEN_SECRET)


# Raspberry Pi pin configuration:
lcd_rs        = 27  # Note this might need to be changed to 21 for older revision Pi's.
lcd_en        = 22
lcd_d4        = 25
lcd_d5        = 24
lcd_d6        = 23
lcd_d7        = 17
lcd_backlight = 4

GREEN_LED = 26
GPIO.setup(GREEN_LED, GPIO.OUT)

class Buzzer(object):
    def __init__(self):
        GPIO.setmode(GPIO.BCM)
        self.buzzer_pin = 18 #set to GPIO pin 5
        GPIO.setup(self.buzzer_pin, GPIO.IN)
        GPIO.setup(self.buzzer_pin, GPIO.OUT)
        print("buzzer ready")

    def __del__(self):
        class_name = self.__class__.__name__
        print (class_name, "finished")

    def buzz(self,pitch, duration):   #create the function buzz and feed it the pitch and duration)

        if(pitch==0):
            time.sleep(duration)
            return
        period = 1.0 / pitch     #in physics, the period (sec/cyc) is the inverse of the frequency (cyc/sec)
        delay = period / 2     #calcuate the time for half of the wave
        cycles = int(duration * pitch)   #the number of waves to produce is the duration times the frequency

        for i in range(cycles):    #start a loop from 0 to the variable cycles calculated above
            GPIO.output(self.buzzer_pin, True)   #set pin 18 to high
            time.sleep(delay)    #wait with pin 18 high
            GPIO.output(self.buzzer_pin, False)    #set pin 18 to low
            time.sleep(delay)    #wait with pin 18 low

    def play(self):
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.buzzer_pin, GPIO.OUT)
        x=0

        pitches=[392,294,0,392,294] #,0,392,0,392,392,392,0,1047,262]
        duration=[0.2,0.2,0.2,0.2,0.2] #,0.2,0.1,0.1,0.1,0.1,0.1,0.1,0.8,0.4]
        for p in pitches:
            self.buzz(p, duration[x])  #feed the pitch and duration to the func$
            time.sleep(duration[x] *0.5)
            x+=1

        GPIO.setup(self.buzzer_pin, GPIO.IN)

buzzer = Buzzer()


# BeagleBone Black configuration:
# lcd_rs        = 'P8_8'
# lcd_en        = 'P8_10'
# lcd_d4        = 'P8_18'
# lcd_d5        = 'P8_16'
# lcd_d6        = 'P8_14'
# lcd_d7        = 'P8_12'
# lcd_backlight = 'P8_7'

# Define LCD column and row size for 16x2 LCD.
lcd_columns = 16
lcd_rows    = 2

# Alternatively specify a 20x4 LCD.
# lcd_columns = 20
# lcd_rows    = 4

# Initialize the LCD using the pins above.
lcd = LCD.Adafruit_CharLCD(lcd_rs, lcd_en, lcd_d4, lcd_d5, lcd_d6, lcd_d7,
                           lcd_columns, lcd_rows, lcd_backlight)

for line in api.GetStreamFilter(track=USERS):
    name = line['user']['name']
    text = line['text']
    print('New message from ' + name + ' : ' + text)
    chunk = chunkstring(text, 16)
    previous = '> @' + name
    buzzer.play()
    for e in chunk:
        lcd.clear()
        GPIO.output(GREEN_LED, True)
        lcd.message(previous + '\n' + e)
        time.sleep(0.5)
        GPIO.output(GREEN_LED, False)
        time.sleep(0.5)
        previous = e
#lcd.message('Blink cursor')
#time.sleep(5.0)
# Stop blinking and showing cursor.
#lcd.show_cursor(False)
