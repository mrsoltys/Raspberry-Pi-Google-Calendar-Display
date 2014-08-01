#!/usr/bin/python
#
# HD44780 LCD Test Script for
# Raspberry Pi
#
# Adapted From: Matt Hawkins
# 		http://www.raspberrypi-spy.co.uk
#
# Date   : 26/07/2012
#
 
# The wiring for the LCD is as follows:
# 1 : GND			-Pi-06 GND
# 2 : 5V			-Pi-02 5V
# 3 : Contrast (0-5V)*		-Pi-02 w/ potentiometer
# 4 : RS (Register Select)	-Pi-26 
# 5 : R/W (Read Write)       	-Pi-06 GND
# 6 : Enable or Strobe		-Pi-24
# 7 : Data Bit 0             
# 8 : Data Bit 1             
# 9 : Data Bit 2             
# 10: Data Bit 3             
# 11: Data Bit 4		-Pi-22
# 12: Data Bit 5		-Pi-18
# 13: Data Bit 6		-Pi-16
# 14: Data Bit 7		-Pi-12
# 15: LCD Backlight 		-Pi-02 5V
# 16: LCD Backlight 		-Pi-06 GND
 
#import
import RPi.GPIO as GPIO
import time
 
# Define GPIO to LCD mapping
GPIO.setmode(GPIO.BOARD)
LCD_RS = 26
LCD_E  = 24
LCD_D4 = 22
LCD_D5 = 18
LCD_D6 = 16
LCD_D7 = 12
 
# Define some device constants
LCD_WIDTH = 16    # Maximum characters per line
LCD_CHR = True
LCD_CMD = False
 
LCD_LINE_1 = 0x80 # LCD RAM address for the 1st line
LCD_LINE_2 = 0xC0 # LCD RAM address for the 2nd line 
 
# Timing constants
E_PULSE = 0.00005
E_DELAY = 0.00005
 
def main():
  # Main program block
 
  #Set Pins as output
  GPIO.setup(LCD_E, GPIO.OUT)  # E
  GPIO.setup(LCD_RS, GPIO.OUT) # RS
  GPIO.setup(LCD_D4, GPIO.OUT) # DB4
  GPIO.setup(LCD_D5, GPIO.OUT) # DB5
  GPIO.setup(LCD_D6, GPIO.OUT) # DB6
  GPIO.setup(LCD_D7, GPIO.OUT) # DB7
 
  # Initialise display
  lcd_init()
 
  # Send some test
  lcd_byte(LCD_LINE_1, LCD_CMD)
  lcd_string("Prof Mike Soltys")
  lcd_byte(LCD_LINE_2, LCD_CMD)
  lcd_string("gCal Checker")
 
def lcd_init():
  # Initialise display
  lcd_byte(0x33,LCD_CMD)
  lcd_byte(0x32,LCD_CMD)
  lcd_byte(0x28,LCD_CMD)
  lcd_byte(0x0C,LCD_CMD)
  lcd_byte(0x06,LCD_CMD)
  lcd_byte(0x01,LCD_CMD)  
 
def lcd_string(message):
  # Send string to display
 
  message = message.ljust(LCD_WIDTH," ")  
 
  for i in range(LCD_WIDTH):
    lcd_byte(ord(message[i]),LCD_CHR)
 
def lcd_byte(bits, mode):
  # Send byte to data pins
  # bits = data
  # mode = True  for character
  #        False for command
 
  GPIO.output(LCD_RS, mode) # RS
 
  # High bits
  GPIO.output(LCD_D4, False)
  GPIO.output(LCD_D5, False)
  GPIO.output(LCD_D6, False)
  GPIO.output(LCD_D7, False)
  if bits&0x10==0x10:
    GPIO.output(LCD_D4, True)
  if bits&0x20==0x20:
    GPIO.output(LCD_D5, True)
  if bits&0x40==0x40:
    GPIO.output(LCD_D6, True)
  if bits&0x80==0x80:
    GPIO.output(LCD_D7, True)
 
  # Toggle 'Enable' pin
  time.sleep(E_DELAY)
  GPIO.output(LCD_E, True)
  time.sleep(E_PULSE)
  GPIO.output(LCD_E, False)
  time.sleep(E_DELAY)      
 
  # Low bits
  GPIO.output(LCD_D4, False)
  GPIO.output(LCD_D5, False)
  GPIO.output(LCD_D6, False)
  GPIO.output(LCD_D7, False)
  if bits&0x01==0x01:
    GPIO.output(LCD_D4, True)
  if bits&0x02==0x02:
    GPIO.output(LCD_D5, True)
  if bits&0x04==0x04:
    GPIO.output(LCD_D6, True)
  if bits&0x08==0x08:
    GPIO.output(LCD_D7, True)
 
  # Toggle 'Enable' pin
  time.sleep(E_DELAY)
  GPIO.output(LCD_E, True)
  time.sleep(E_PULSE)
  GPIO.output(LCD_E, False)
  time.sleep(E_DELAY)   
 
if __name__ == '__main__':
  main()