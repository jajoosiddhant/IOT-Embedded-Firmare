# Smart Car Parking System using Bluetooth Mesh

## Hardware and Software Components:
- Sparkfun Distance Sensor Breakout - RFD77402.(Proximity Sensor)
- Sparkfun Open PIR
- Adafruit Air Quality sensor CCS811
- Real Time Service using on-board RTCC
- Simplicity Studio

## Overview
The Smart Car Parking System aims at simplifying the process of parking in basements. It is designed to notify any incoming drivers of empty parking slots throughout the area. In case of multi-level parking area, each level can have one friend node displaying the
empty slots for that particular level. This is implemented using a Bluetooth Mesh framework of a Low Power Node(LPN) for each parking slot and a Friend node which can support as many LPNs as the application requires. For demonstration purposes, one LPN is connected to one friend node here.

## Problems Addressed
1. The first problem that drivers face in huge parking lots is that they are unaware of the status of different parking slots i.e whether they are vacant or occupied. It becomes very difficult for a driver to keep circling around the parking slot to find a vacant spot for the
car. Sometimes it can be tedious if very few parking slots are available. This results in wastage of time, energy and fuel.
2. The second problem that needs to be addressed in a parking lot is the wastage of electricity due to unwanted use of lights. The parking slots do not need lights to be switched on all the time. Lights are necessarily required when a person parks/approaches the car.
3. The third problem is that of fires in an enclosed area, which aren’t easily detected.

## Solutions Proposed
The project addresses each problem by:
1. Displaying the status of different parking slots or the number of parking slots vacant in a particular area at the point of entry of vehicles on an LCD. In this way, any driver on entry can see which slot is empty and directly move to that particular slot without the
headache of searching for a vacancy. To accomplish this, we are using a proximity sensor at each slot to determine whether a car is parked or not, allowing us to determine vacancy of that slot.
2. Switching on the lights (and thus saving energy) only when there is a car being parked or unparked or if we detect any motion in the respective parking slot. This is done through the use of a motion sensor at each parking slot which detects an incoming or exiting car, or even a person approaching his/her car. Also, an added feature is switching on the lights at night to allow the security cameras to continue taping. This can be done by fetching the time of the day and switching on or off the lights by checking the time using publish and subscribe. In case of an emergency, all the lights can be switched on or off by a button press located near the LCD as well.
3. Using a fire detection system. A single fire sensor can be deployed on each friend node to continuously check for fires. If a fire is detected, a “publish and subscribe” message can be used to toggle all LEDs in the parking lot. As each friend node is common to a number of parking slots, this problem is solved as well.

## Nodes used in the System
### 1. Low Power Node (LPN)
- This is a Low Power Node (Blue Gecko) with Server Model.
- Server Model uses the Generic ONOFF Mesh Model.
- Server Model which stores the state of the Parking Slot value depending upon the Motion Sensor and the Proximity Sensor and turns on the light(LEDs) accordingly.
- The sensors used are:
1. Sparkfun Distance Sensor Breakout - RFD77402.(Proximity
Sensor) (Quantity-1), cost - $ 12.71
2. Sparkfun Open PIR - $ 14.95 (Quantity -1) (Could not find the
p/n number on the website )

### 2. Friend Node
This is a Friend Node(Blue Gecko) with Control Model.
- Server Model which stores the CO2 value using the Air Quality Sensor.
- Client Model which can control the light(LEDs) depending on time of day and by using the push buttons.
- The Sensor used is:
1. Adafruit Air Quality sensor CCS811
- The Service used is:
1. Real Time Service using on-board RTCC

## Block Diagram
<p align="middle">
<img src="Low%20Power%20Node/Images/Block%20Diagram.PNG">
</p>

## Project Summary
In summary, the Smart Car Parking System is ready to be deployed in a car park area. It has been tested for use, and detects motion immediately, resulting in a light on the Parking Slot being turned on for visibility. As soon as motion is detected, proximity data
is calculated on the LPN’s side, and if below a certain threshold, data is sent to the Friend node, which displays the status on an LCD. The Friend calculates the time of day based on an RTCC (Real TIme Clock and Calendar), and sends a set message to the LPN to conserve energy by going into Low Power mode (during nighttime) or to wake up more frequently and take readings (during daytime). The Friend also monitors for any fires in the vicinity, and in case of one, sends the fire status to all the other Friends throughout the parking lot. The project functionality has been tested, and is ready for deployment. The original problems which the project aimed at addressing have been appropriately solved, and have been done so in a way so as to conserve energy as well.

The Code Zip file for Low Power Node can be found [HERE](https://github.com/jajoosiddhant/IOT-Embedded-Firmware/tree/master/BT_MESH_Smart_Car_Parking/Low%20Power%20Node/Code%20Zip%20File).

The entire Project Report can be found [HERE](https://github.com/jajoosiddhant/IOT-Embedded-Firmware/tree/master/BT_MESH_Smart_Car_Parking/Low%20Power%20Node/_Project_Report).

The entire Low Power Node Report can be found [HERE](https://github.com/jajoosiddhant/IOT-Embedded-Firmware/tree/master/BT_MESH_Smart_Car_Parking/Low%20Power%20Node/_Project_Report).

