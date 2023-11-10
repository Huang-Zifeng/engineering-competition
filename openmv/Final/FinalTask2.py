# Final Task - By: Mitcher - Sun Oct 16 2022

import sensor, image, time
from pyb import LED,UART
import lcd

def get_qrcode_seq(str):
    list = []
    for i in str:
        if i.isdigit():
            list.append(int(i))
    return list

def get_color_seq(a, b, c): # a是比较对象,b和c是被比较对象,返回值i是a在a,b,c中的排名(即物料在货架上的位置)
    if a < b and a < c:
        i = 1
    elif a > b and a > c:
        i = 3
    else:
        i = 2
    return int(i)

thresholds = [(30, 100, 15, 127, 15, 127), # red
              (19 , 36 , -36 , -23 , 18 , 37), # green (48 , 61 , -49 , -41 , 27 , 40)
              (5 , 12 , -15 , -4 , -6 , 8)] # blue 9, 16, -6, 8, -31, -12

uart = UART(3, 115200)

red_led   = LED(1)
green_led = LED(2)
blue_led  = LED(3)
ir_led    = LED(4)

cx_r = 0
cx_g = 0
cx_b = 0

ROI=(0, 0, 320, 120)

qrcode_flag = False
blobs_flag = False
counter = 0
r_flag = False
g_flag = False
b_flag = False

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)
lcd.init()

while(True):
    img = sensor.snapshot()
    codes = img.find_qrcodes()
    green_led.on()
    if codes:
        for code in img.find_qrcodes():
            qrcode_seq = get_qrcode_seq(code[4])
            qrcode_flag = True
        FH = bytearray([0x2C, 0x12, qrcode_seq[0], qrcode_seq[1], qrcode_seq[2], qrcode_seq[3], qrcode_seq[4], qrcode_seq[5], 0x5B])
        for i in range(5):
            print(FH)
            uart.write(FH)
            time.sleep_ms(10)
        img.draw_rectangle(code[0:4], color=(255, 0, 0)) # rect
        img = image.Image(128,160,sensor.BINARY)
        img.draw_string(10, 50, code[4],scale=2)
        lcd.display(img)
        print(qrcode_seq)
        green_led.off()
        time.sleep_ms(50)
        img.clear()
        break

while(True):
    blue_led.on()
    if uart.any():
        blue_led.off()
        break

while(True):
    red_led.on()
    img = sensor.snapshot()
    #img.lens_corr(1.8)
    img.draw_rectangle((0,120,320,120),color=(0,0,0),fill=True)
    for blob in img.find_blobs(thresholds, pixels_threshold=100, area_threshold=100, merge=False):
        if blob.code() == 1: # r code
            img.draw_rectangle(blob.rect(),color=(255,0,0))
            img.draw_cross(blob.cx(), blob.cy())
            cx_r = blob.cx()
            r_flag = True
        if blob.code() == 2: # g code
            img.draw_rectangle(blob.rect(),color = (0,255,0))
            img.draw_cross(blob.cx(), blob.cy())
            cx_g = blob.cx()
            g_flag = True
        if blob.code() == 4: # b code
            img.draw_rectangle(blob.rect(),color = (0,0,255))
            img.draw_cross(blob.cx(), blob.cy())
            cx_b = blob.cx()
            b_flag = True
        if r_flag and g_flag and b_flag:
            color_seq = [get_color_seq(cx_r, cx_g, cx_b), get_color_seq(cx_g, cx_r, cx_b), get_color_seq(cx_b, cx_g, cx_r)]
            grab_seq = [color_seq[qrcode_seq[0]-1], color_seq[qrcode_seq[1]-1], color_seq[qrcode_seq[2]-1], 0, 0, 0]
            FH = bytearray([0x2C, 0x12, grab_seq[0], grab_seq[1], grab_seq[2], 0, 0, 0, 0x5B])
            for i in range(5):
                print(FH)
                uart.write(FH)
                blobs_flag = True
                time.sleep_ms(10)
        break
    red_led.off();
    if blobs_flag:
        break

while(True):
    blue_led.on()
    if uart.any():
        blue_led.off()
        break

while(True):
    red_led.on()
    img = sensor.snapshot()
    img.draw_rectangle((0,0,320,120),color=(0,0,0),fill=True)
    for blob in img.find_blobs(thresholds, pixels_threshold=100, area_threshold=100, merge=False):
        if blob.code() == 1: # r code
            img.draw_rectangle(blob.rect(),color=(255,0,0))
            img.draw_cross(blob.cx(), blob.cy())
            cx_r = blob.cx()
        if blob.code() == 2: # g code
            img.draw_rectangle(blob.rect(),color = (0,255,0))
            img.draw_cross(blob.cx(), blob.cy())
            cx_g = blob.cx()
        if blob.code() == 4: # b code
            img.draw_rectangle(blob.rect(),color = (0,0,255))
            img.draw_cross(blob.cx(), blob.cy())
            cx_b = blob.cx()
        color_seq = [get_color_seq(cx_r, cx_g, cx_b), get_color_seq(cx_g, cx_r, cx_b), get_color_seq(cx_b, cx_g, cx_r)]
        grab_seq = [color_seq[qrcode_seq[3]-1], color_seq[qrcode_seq[4]-1], color_seq[qrcode_seq[5]-1]]
        FH = bytearray([0x2C, 0x12, grab_seq[0], grab_seq[1], grab_seq[2], 0, 0, 0, 0x5B])
        tiem.sleep_ms(10)
        counter = counter+1
        if counter > 20:
            for i in range(5):
                print(FH)
                uart.write(FH)
                blobs_flag = True
                time.sleep_ms(10)
        break
    red_led.off();
    if blobs_flag:
        break








