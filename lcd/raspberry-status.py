#!/usr/bin/python
# Example using a character LCD connected to a Raspberry Pi or BeagleBone Black.
import math
import time
import psutil as p
import Adafruit_CharLCD as LCD


# Raspberry Pi pin configuration:
lcd_rs        = 27  # Note this might need to be changed to 21 for older revision Pi's.
lcd_en        = 22
lcd_d4        = 25
lcd_d5        = 24
lcd_d6        = 23
lcd_d7        = 18
lcd_onOff     = 12
lcd_backlight = 4

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
							lcd_columns, lcd_rows, lcd_onOff, lcd_backlight)

lcd.turnOn()

lcd.message('Process:' + str(len(p.pids())) )
time.sleep(5.0)
lcd.clear()

lcd.message('CPU : ' + str(p.cpu_percent(interval=1.0)) + '%\nMemory : ' + str(p.virtual_memory().percent)+'%')
time.sleep(5.0)
lcd.clear()

established_conns = [conn for conn in p.net_connections() if conn.status == 'ESTABLISHED']


lcd.message('Current conns');
time.sleep(5.0)
lcd.clear()

for c in established_conns:
    message = 'raddr : ' + str(c.raddr) +'\nladdr : ' + str(c.laddr)
    lcd.message(message)
    for i in range(lcd_columns-len(message)):
        time.sleep(0.5)
        lcd.move_right()
    lcd.clear()

for disk in p.disk_partitions():
    percent = p.disk_usage(disk.mountpoint).percent
    lcd.message('Use of ' + disk.mountpoint+'\n'+str(percent)+'%')
    time.sleep(5.0)
    lcd.clear()

lcd.turnOff()
