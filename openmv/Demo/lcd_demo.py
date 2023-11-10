# Untitled - By: Mitcher - Sun Oct 30 2022

import sensor, image, time
import lcd

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA2)
sensor.skip_frames(time = 2000)
lcd.init()

while(True):
    img = image.Image(128,160,sensor.BINARY) #生成一张黑色图
    img = img.draw_string(10,10,"hello world")
    lcd.display(img)
