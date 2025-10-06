# Network Bridge Simulation with Store-and-Forward Algorithm  

**Full Name:**  
Network Bridge Simulation with Store-and-Forward Algorithm - Dynamic Relay for Ethernet and Ageing Management (DREAM)

---

## Description  
DREAM is a small C++ program that simulates the behaviour of a network bridge using the store-and-forward algorithm. It dynamically learns MAC addresses and their associated ports, keeps an updated MAC table, and forwards Ethernet frames to the correct port or broadcasts them if the destination is unknown.  

The program also removes outdated entries automatically to keep the MAC table efficient. This is a simple but useful demonstration of how real network bridges work.  

---

## Features  
- Learns MAC addresses and ports dynamically.  
- Forwards frames to the correct port when known.  
- Broadcasts frames when the destination MAC is unknown.  
- Automatically cleans MAC table entries older than 300 seconds.  
- Ignores invalid inputs.  

---

## How to Run  
1. Clone the repository:  
   ```bash
   git clone https://github.com/pirasfrancesco6492/DREAM-NetworkBridge.git
   cd DREAM-NetworkBridge
   ```  
2. Compile the program:  
   ```bash
   g++ -o DREAM bridge.cpp
   ```  
3. Run the program:  
   ```bash
   ./DREAM
   ```  
4. Provide input lines in this format:  
   ```
   XX:XX:XX:XX:XX:XX,port,XX:XX:XX:XX:XX:XX
   ```  
   Example:  
   ```
   00:1A:2B:3C:4D:5E,1000,01:1A:2B:3C:4D:5F
   ```  
5. Type `stop` to end the program.  

---

## Example Output  
```
[BRIDGE MODULE] Initializing Store-and-Forward Algorithm...
[FILTERING DECISION] Forwarding frame to port 1000
[MAINTENANCE] Cleaning MAC table: removing stale entries older than 300 seconds.
```

---

## Easter Egg  
The acronym **DREAM** stands for *Dynamic Relay for Ethernet and Ageing Management*.  
It is a hidden nod to the inspiration behind this project — because I actually envisioned it in a dream.
