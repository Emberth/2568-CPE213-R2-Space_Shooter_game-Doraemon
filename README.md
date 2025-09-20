# Space Shooter with Ultrasonic Detection

## Project Overview
This project is a **console-based space shooter game** implemented on ESP32.  
It uses:
- **Potentiometer** to control the spaceship movement.
- **Button** to shoot bullets.
- **Two ultrasonic sensors** to detect a person within 10 cm — the game only runs if a player is detected.
- **LED indicators** to display lives remaining.

The game logic runs on the ESP32 and is displayed via the Serial Monitor.

---
## picture of actual hardware

![Uploading IMG_0635.jpg…]()

---

## Features
- Spaceship movement with potentiometer.
- Shooting with button (multi-bullet support).
- Asteroids with different movement patterns:
  - `*` normal
  - `@` zigzag
  - `#` fast falling
- Dynamic difficulty scaling:
  - Asteroids increase in speed and number as score increases.
- LED life indicator (3 LEDs).
- Automatic game over state with score display.

---

## How to Run
1. Connect hardware according to the provided circuit diagram.  
2. Upload code to ESP32 via Arduino IDE.  
3. Double-click `launch_game.bat` to open the serial monitor at 921600 baud.  
4. Place hand/person within 10 cm of ultrasonic sensor to start the game.  

---

## presentation link
- https://www.canva.com/design/DAGzghaKBDA/ZtQdp-C0_UyI1fh8lkaYUA/edit?utm_content=DAGzghaKBDA&utm_campaign=designshare&utm_medium=link2&utm_source=sharebutton

---
