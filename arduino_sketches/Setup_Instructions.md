# Instructions for Synchronizing Clocks on the Complex Wheels

0) Library Installation  
Download the following libraries (**Sketch -> Include Library -> Manage Libraries**) (only needed the first time):

    - `TimerThree`
    - `RTClib` (by Adafruit)
    - `SDfat` (by Bill Greiman – **NOT** the Adafruit fork)

1) Connect the Arduino via USB cable  
2) Open both sketches: `Sync_clock` and `ActiWheel`  
3) Select `Sync_clock` and open the Serial Monitor (magnifying glass icon in the top right)  
4) Click **Upload**  
5) Once upload is complete, you should see the current date and time printed in the Serial Monitor  
6) **Without closing or reopening the Serial Monitor**, select the `ActiWheel` sketch  
7) Modify line 57:

```cpp
sprintf(filename, "Wheel1_%04d-%02d-%02d_%02d-%02d-%02d.dat", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
```

Change the number in `Wheel1` according to the wheel you are using. This ensures that the filename includes the wheel number and avoids confusion when analyzing data from different wheels.

8) Click **Upload**  
9) The wheel is now ready

---

#### Note

The `Sync_clock` sketch forces the wheel’s internal clock to synchronize with the computer’s current time at the moment of compilation and upload. Every time the Arduino restarts, the clock is reset to that exact time.
For example:  
If you upload the sketch at 16:00, the clock will read 16:00.  
If you restart the Arduino at 16:10, after reboot the clock will again read 16:00.  
Therefore, it is important to upload the next sketch **without restarting the Arduino**, **disconnecting it**, or **closing/reopening the Serial Monitor**.
