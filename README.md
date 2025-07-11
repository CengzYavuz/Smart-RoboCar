
# 🏁 Smart Arduino Car — ETE3007 Robot Challenge Spring 2025

This project is an **autonomous smart car** developed for the DEU ETE3007 Fundamentals of Robotics Robot Challenge (Spring 2025). The car uses a **modular event-driven control system**, relying solely on onboard sensors and logic to navigate the competition track featuring hills, wall obstacles, and color zones.

---

## 🎯 Objective

Build an autonomous mobile robot capable of navigating the challenge track from **start to finish**, without remote control, in accordance with competition rules. The robot must:

- Start autonomously after a reset
- Pass over a hill and through three wall obstacles
- React accurately to colored track zones
- Reach the finish line while earning checkpoints and time bonuses

---

## 🔧 Hardware Used

| Component               | Description                               |
|-------------------------|-------------------------------------------|
| 🧠 Arduino Uno          | Main microcontroller                      |
| ⚙️ Adafruit Motor Shield| Controls 4 DC motors                      |
| 🚗 4x DC Motors          | Provides locomotion                       |
| 📏 Ultrasonic Sensor     | Used for obstacle detection and alignment |
| 🔋 Battery Pack          | Powers motors and Arduino                 |

---

## 🧠 Software Architecture

The robot is driven by a **custom event queue system**, where each event contains:

- An **action function** (what the robot should do)
- A **condition function** (when to do it)
- An optional **duration** (how long the action should run)

```cpp
struct Event {
  ActionFunction action;
  ConditionFunction condition;
  unsigned long duration; // in ms
};
```

### ✅ Features

- **Event Queue Logic**: Modular movement instructions that run under given conditions or time constraints
- **Distance Alignment**: `AdjustMiddle()` aligns the robot to a target distance using PID-like speed control
- **Actions Supported**:
  - Forward, Backward
  - Left and Right Turn
  - Stop
- **Obstacle Handling**: Ultrasonic sensor helps maintain spacing from obstacles (walls, checkpoints)

---

## 🛠 How to Use

1. Connect hardware as follows:
   - Motors to M1–M4 on AFMotor Shield
   - Ultrasonic sensor:
     - `TRIG_PIN` → A1
     - `ECHO_PIN` → A0
2. Upload the sketch to the Arduino.
3. Place robot at the start line.
4. Press the **reset button** to start execution after referee’s start signal.

---

## 🧪 Behavior Flow

The robot proceeds through a sequence of actions such as:

- Drive forward for specified durations
- Use `AdjustMiddle()` to align distance (e.g., before a wall or obstacle)
- Turn left or right at specific checkpoints
- Stop at the finish line

Example event:
```cpp
addEvent(
  []() { AdjustMiddle(26); },
  []() { return CorrectFlag; },
  0
);
```

---

## 📊 Scoring Strategy

The robot is designed to maximize the following:

- ✅ **Passing Checkpoints** (walls and hill)
- ⏱ **Time Efficiency** via optimized speed and smooth transitions
- 🤖 **Complexity & Embedded Control** using event-based logic
- 📏 **Distance Adjustments** for obstacle safety
- 🎨 Future additions may enhance **aesthetics** if needed

---

## 📁 Project Structure

```bash
smart-car/
├── smart_car.ino         # Main Arduino code
├── README.md             # Project documentation (this file)
└── competition_doc.pdf   # ETE3007 competition guidelines
```

---

## 📌 Rules Compliance

✅ No remote control used  
✅ Fully autonomous and self-contained  
✅ Communication modules (Bluetooth/WiFi) not used during execution  
✅ Final reset logic triggered manually before starting

---

## 📜 License

MIT License – free to use, modify, and learn from for educational and competitive purposes.
