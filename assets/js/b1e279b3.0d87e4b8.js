"use strict";(self.webpackChunkacps_documentation=self.webpackChunkacps_documentation||[]).push([[4220,5258],{8748:(e,n,i)=>{i.r(n),i.d(n,{assets:()=>x,contentTitle:()=>m,default:()=>f,frontMatter:()=>p,metadata:()=>u,toc:()=>g});var s=i(4848),t=i(8453);const r=i.p+"assets/images/Motion-2e2b21965a7a2120ca1549fa7a334de8.jpg",o=i.p+"assets/images/ESP8266-8e966d8f437d01ae0bb4a1fad19804b8.png",a=i.p+"assets/images/PIR-92b3fdb673b99a8d4e084bdf83396207.jpg";var l=i(1885),c=i(7333),d=i(380);const h=i.p+"assets/images/Motion-Detector_Circuit_Diagram_bb-94d74e4b9a2a2ed15cbdc850b7e2f5b8.png",p={title:"Node 7: Motion Detector",description:"Enhances security by detecting and alerting to motion within its vicinity, suitable for homes and commercial spaces."},m="Node 7: Motion Detector",u={id:"physical_components/node7",title:"Node 7: Motion Detector",description:"Enhances security by detecting and alerting to motion within its vicinity, suitable for homes and commercial spaces.",source:"@site/docs/physical_components/node7.mdx",sourceDirName:"physical_components",slug:"/physical_components/node7",permalink:"/docs/physical_components/node7",draft:!1,unlisted:!1,tags:[],version:"current",frontMatter:{title:"Node 7: Motion Detector",description:"Enhances security by detecting and alerting to motion within its vicinity, suitable for homes and commercial spaces."},sidebar:"docs",previous:{title:"Node 6: Local Weather Station",permalink:"/docs/physical_components/node6"},next:{title:"Node 8: Obstacle Detector",permalink:"/docs/physical_components/node8"}},x={},g=[{value:"Components Summary",id:"components-summary",level:2},{value:"Components Overview",id:"components-overview",level:2},{value:"Controller: ESP8266 Development Board (NodeMCU)",id:"controller-esp8266-development-board-nodemcu",level:3},{value:"Technical Specifications and Features:",id:"technical-specifications-and-features",level:4},{value:"References:",id:"references",level:4},{value:"Sensor: PIR Motion Sensor",id:"sensor-pir-motion-sensor",level:3},{value:"Technical Specifications and Features:",id:"technical-specifications-and-features-1",level:4},{value:"References:",id:"references-1",level:4},{value:"Actuator: LED",id:"actuator-led",level:3},{value:"Technical Specifications and Features",id:"technical-specifications-and-features-2",level:4},{value:"References",id:"references-2",level:4},{value:"Actuator: Servo Motor - Tower Pro SG90",id:"actuator-servo-motor---tower-pro-sg90",level:3},{value:"Technical Specifications and Features",id:"technical-specifications-and-features-3",level:4},{value:"References",id:"references-3",level:4},{value:"Communication Protocols",id:"communication-protocols",level:2},{value:"Controller Communication",id:"controller-communication",level:3},{value:"Wi-Fi Connection:",id:"wi-fi-connection",level:4},{value:"MQTT Protocol:",id:"mqtt-protocol",level:4},{value:"Sensor Communication",id:"sensor-communication",level:3},{value:"Pin Connections",id:"pin-connections",level:2},{value:"PIR Sensor Module Pin Connections",id:"pir-sensor-module-pin-connections",level:3},{value:"Green LED Pin Connections",id:"green-led-pin-connections",level:3},{value:"Servo Motor Pin Connections",id:"servo-motor-pin-connections",level:3},{value:"Circuit Diagram",id:"circuit-diagram",level:2},{value:"Link to Code Explanation and Tutorial",id:"link-to-code-explanation-and-tutorial",level:2}];function j(e){const n={a:"a",h1:"h1",h2:"h2",h3:"h3",h4:"h4",li:"li",p:"p",span:"span",strong:"strong",ul:"ul",...(0,t.R)(),...e.components};return(0,s.jsxs)(s.Fragment,{children:[(0,s.jsx)(n.h1,{id:"node-7-motion-detector",children:"Node 7: Motion Detector"}),"\n",(0,s.jsx)(n.p,{children:"The Motion Detector, equipped with an ESP8266, detects movement using a PIR sensor, making it an essential component for security systems in homes and businesses. This node's functionality highlights its role in automated security setups, where real-time motion detection triggers alerts and recordings, enhancing the safety and surveillance capabilities of the system."}),"\n",(0,s.jsx)("img",{src:r,alt:"Motion Detector",style:{width:"75%",display:"block",margin:"20px auto"}}),"\n",(0,s.jsx)(n.h2,{id:"components-summary",children:"Components Summary"}),"\n",(0,s.jsx)("div",{style:{display:"flex",justifyContent:"center",margin:"20px"},children:(0,s.jsxs)("table",{style:{width:"75%",borderCollapse:"collapse"},children:[(0,s.jsx)("thead",{children:(0,s.jsxs)("tr",{children:[(0,s.jsx)("th",{style:{padding:"8px",width:"30%",textAlign:"center"},children:"Component Type"}),(0,s.jsx)("th",{style:{padding:"8px",width:"70%",textAlign:"center"},children:"Component Name"})]})}),(0,s.jsxs)("tbody",{children:[(0,s.jsxs)("tr",{children:[(0,s.jsx)("td",{style:{padding:"8px",textAlign:"right"},children:"Controller"}),(0,s.jsx)("td",{style:{padding:"8px"},children:(0,s.jsx)("a",{href:"#controller-esp8266-development-board-nodemcu",children:"ESP8266 Dev Board (NodeMCU)"})})]}),(0,s.jsxs)("tr",{children:[(0,s.jsx)("td",{style:{padding:"8px",textAlign:"right"},children:"Sensor"}),(0,s.jsx)("td",{style:{padding:"8px"},children:(0,s.jsx)("a",{href:"#sensor-pir-motion-sensor",children:"PIR Motion Sensor"})})]}),(0,s.jsxs)("tr",{children:[(0,s.jsx)("td",{style:{padding:"8px",textAlign:"right"},children:"Actuator"}),(0,s.jsx)("td",{style:{padding:"8px"},children:(0,s.jsx)("a",{href:"#actuator-servo-motor---tower-pro-sg90",children:"Servo Motor"})})]}),(0,s.jsxs)("tr",{children:[(0,s.jsx)("td",{}),(0,s.jsx)("td",{style:{padding:"8px"},children:(0,s.jsx)("a",{href:"#actuator-led",children:"Green LED"})})]})]})]})}),"\n",(0,s.jsx)(n.h2,{id:"components-overview",children:"Components Overview"}),"\n",(0,s.jsx)(n.h3,{id:"controller-esp8266-development-board-nodemcu",children:"Controller: ESP8266 Development Board (NodeMCU)"}),"\n",(0,s.jsx)(n.p,{children:"The ESP8266 NodeMCU is a popular and cost-effective development board that excels in a wide range of IoT and embedded applications. Its standout feature is the built-in Wi-Fi connectivity, enabling seamless wireless communication for IoT devices. With a compact form factor and compatibility with the Arduino IDE, it simplifies the development process for IoT projects, including home automation, smart appliances, sensor networks, and remote monitoring systems. The NodeMCU is known for its low power consumption, making it suitable for battery-operated devices. Additionally, it supports secure connections, making it ideal for applications where data privacy is paramount, such as in smart locks, surveillance cameras, and IoT-based security systems. Its affordability, community support, and extensive capabilities continue to make it a top choice for IoT enthusiasts and professionals alike."}),"\n",(0,s.jsx)("img",{src:o,alt:"ESP8266",style:{width:"75%",display:"block",margin:"20px auto"}}),"\n",(0,s.jsx)(n.h4,{id:"technical-specifications-and-features",children:"Technical Specifications and Features:"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Wi-Fi Certification"}),": Certified by Wi-Fi Alliance."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Protocols"}),": Supports 802.11 b/g/n (HT20) Wi-Fi protocols."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Frequency Range"}),": Operates in the 2.4 GHz to 2.5 GHz frequency range (2400 MHz to 2483.5 MHz)."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Transmission Power"}),":","\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsx)(n.li,{children:"802.11 b: +20 dBm"}),"\n",(0,s.jsx)(n.li,{children:"802.11 g: +17 dBm"}),"\n",(0,s.jsx)(n.li,{children:"802.11 n: +14 dBm"}),"\n"]}),"\n"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Receiver Sensitivity"}),":","\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsx)(n.li,{children:"802.11 b: -91 dBm (11 Mbps)"}),"\n",(0,s.jsx)(n.li,{children:"802.11 g: -75 dBm (54 Mbps)"}),"\n",(0,s.jsx)(n.li,{children:"802.11 n: -72 dBm (MCS7)"}),"\n"]}),"\n"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Antenna"}),": Supports various antenna options, including PCB Trace, External, IPEX Connector, and Ceramic Chip."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Hardware"}),":","\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsx)(n.li,{children:"CPU: Tensilica L106 32-bit processor"}),"\n",(0,s.jsx)(n.li,{children:"Peripheral Interface: UART/SDIO/SPI/I2C/I2S/IR Remote Control"}),"\n",(0,s.jsx)(n.li,{children:"GPIO/ADC/PWM/LED Light & Button"}),"\n"]}),"\n"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Operating Voltage"}),": 2.5V to 3.6V."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Operating Current"}),": Average current consumption is approximately 80 mA."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Operating Temperature Range"}),": Operates reliably in the temperature range from -40\xb0C to 125\xb0C."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Package Size"}),": Comes in a compact QFN32-pin package measuring 5mm x 5mm."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Firmware Upgrade"}),": Supports firmware upgrades via UART download or OTA (over-the-air) using network connections."]}),"\n"]}),"\n",(0,s.jsx)(n.h4,{id:"references",children:"References:"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.a,{target:"_blank","data-noBrokenLinkCheck":!0,href:i(6301).A+"",children:"ESP8266 Datasheet"})}),"\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.a,{target:"_blank","data-noBrokenLinkCheck":!0,href:i(9459).A+"",children:"ESP8266 Development Board Pinout"})}),"\n"]}),"\n",(0,s.jsx)(n.h3,{id:"sensor-pir-motion-sensor",children:"Sensor: PIR Motion Sensor"}),"\n",(0,s.jsx)(n.p,{children:"A Passive Infrared (PIR) sensor is an electronic sensor that measures infrared light radiating from objects in its field of view. They are most often used in PIR-based motion detectors. PIR sensors are commonly used in security alarms and automatic lighting applications."}),"\n",(0,s.jsx)("img",{src:a,alt:"PIR Motion Sensor",style:{width:"75%",display:"block",margin:"20px auto"}}),"\n",(0,s.jsx)(n.h4,{id:"technical-specifications-and-features-1",children:"Technical Specifications and Features:"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Operating Voltage"}),": 3V\u20135V"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Operating Current (VCC = 3V)"}),": 100uA"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Operating Current (VCC = 5V)"}),": 150uA"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Measuring Range"}),": 0.1 - 6m"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Default Detecting Distance"}),": 3m"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Holding Time"}),": 1 - 25s"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Working Wave Length"}),": 7 - 14um"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Detecting Angle"}),": 120 degrees"]}),"\n"]}),"\n",(0,s.jsx)(n.h4,{id:"references-1",children:"References:"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.a,{target:"_blank","data-noBrokenLinkCheck":!0,href:i(3898).A+"",children:"PIR Motion Sensor Datasheet"})}),"\n"]}),"\n",(0,s.jsx)(n.h3,{id:"actuator-led",children:"Actuator: LED"}),"\n",(0,s.jsx)(n.p,{children:"LEDs, or Light Emitting Diodes, are semiconductor devices that emit light when an electric current is applied. Known for their energy efficiency, compact size, and long life, LEDs are extensively used across various applications including electronic devices, display panels, indicators, and general lighting. Their ability to be manufactured in various colors and sizes makes LEDs exceptionally versatile for design and functional purposes in numerous projects."}),"\n",(0,s.jsx)("img",{src:l.A,alt:"Green LED",style:{width:"25%",display:"block",margin:"20px auto",transform:"scaleX(-1)"}}),"\n",(0,s.jsx)(n.h4,{id:"technical-specifications-and-features-2",children:"Technical Specifications and Features"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Dice Material"}),": Gallium Arsenide Phosphide on Gallium Phosphide (GaAsP on GaP/GaP)."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Emitted Colors"}),": Available in Green and Red."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Lens Color"}),": Green Diffused and Red Diffused."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Peak Wavelength"}),":","\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsx)(n.li,{children:"Green: 568 nm"}),"\n",(0,s.jsx)(n.li,{children:"Red: 587 nm"}),"\n"]}),"\n"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Viewing Angle"}),": 45 degrees, offering focused illumination."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Luminous Intensity (IV)"}),":","\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsx)(n.li,{children:"Green: 15 mcd"}),"\n",(0,s.jsx)(n.li,{children:"Red: 14 mcd"}),"\n"]}),"\n"]}),"\n"]}),"\n",(0,s.jsx)(n.h4,{id:"references-2",children:"References"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.a,{target:"_blank","data-noBrokenLinkCheck":!0,href:i(9007).A+"",children:"Green LED Datasheet"})}),"\n"]}),"\n",(0,s.jsx)(n.h3,{id:"actuator-servo-motor---tower-pro-sg90",children:"Actuator: Servo Motor - Tower Pro SG90"}),"\n",(0,s.jsx)(n.p,{children:"The Tower Pro SG90 9g mini servo motor is a compact and reliable actuator widely used in radio-controlled (RC) applications and projects that require precise movement control. Ideal for robotics, CNC machines, and RC car steering systems, this servo allows for detailed angular position control over a 180\xb0 range using Pulse Width Modulation (PWM). Its lightweight design and high torque capability make it suitable for a wide range of applications that demand precise positioning."}),"\n",(0,s.jsx)("img",{src:c.A,alt:"Servo",style:{width:"50%",display:"block",margin:"20px auto"}}),"\n",(0,s.jsx)(n.h4,{id:"technical-specifications-and-features-3",children:"Technical Specifications and Features"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Operating Voltage"}),": Typically +5V"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Torque"}),": 2.5 kg/cm"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Operating Speed"}),": 0.1 seconds per 60\xb0"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Gear Type"}),": Plastic"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Rotation"}),": 0\xb0 to 180\xb0"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Weight"}),": 9 grams"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Included Accessories"}),": Gear horns and screws"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Cable Length"}),": 15 cm"]}),"\n"]}),"\n",(0,s.jsx)(n.h4,{id:"references-3",children:"References"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.a,{target:"_blank","data-noBrokenLinkCheck":!0,href:i(7006).A+"",children:"Tower Pro SG90 Mini Servo Motor Datasheet"})}),"\n"]}),"\n",(0,s.jsx)(n.h2,{id:"communication-protocols",children:"Communication Protocols"}),"\n",(0,s.jsx)(n.h3,{id:"controller-communication",children:"Controller Communication"}),"\n",(0,s.jsx)(n.p,{children:"The communication between the ESP8266 controller and the ThingsBoard server involves two key protocols: Wi-Fi and MQTT."}),"\n",(0,s.jsx)(n.h4,{id:"wi-fi-connection",children:"Wi-Fi Connection:"}),"\n",(0,s.jsx)(n.p,{children:"The ESP8266 connects to a local Wi-Fi network, enabling it to access the internet. This connection provides the foundation for communication with external services like the ThingsBoard server."}),"\n",(0,s.jsx)(n.h4,{id:"mqtt-protocol",children:"MQTT Protocol:"}),"\n",(0,s.jsx)(n.p,{children:"The Message Queuing Telemetry Transport (MQTT) protocol is employed for transmitting data between the controller and the ThingsBoard server. MQTT is a lightweight and efficient protocol designed for remote devices with limited resources. It facilitates the publication of data (such as access events) from the controller to the server, as well as the subscription to commands or acknowledgements from the server. MQTT ensures secure and reliable communication, making it ideal for real-time data visualization and monitoring on the ThingsBoard dashboard. The controller publishes messages related to access events, which are then visualized on the dashboard, offering insights into the system's performance and access patterns."}),"\n",(0,s.jsx)(n.h3,{id:"sensor-communication",children:"Sensor Communication"}),"\n",(0,s.jsx)(n.p,{children:"In the node, the PIR sensor communicates with the ESP8266 development board controller through digital signal pathways. When motion is detected, the PIR sensor sends a digital signal to the controller, indicating the presence of motion."}),"\n",(0,s.jsx)(n.h2,{id:"pin-connections",children:"Pin Connections"}),"\n",(0,s.jsx)(n.p,{children:"This section provides details about the pin connections between the components used in the node and the ESP8266 controller."}),"\n",(0,s.jsx)(n.h3,{id:"pir-sensor-module-pin-connections",children:"PIR Sensor Module Pin Connections"}),"\n",(0,s.jsx)(d.default,{data:[["PIR Sensor","","ESP8266 Dev Board"],["VCC",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-red"})}),"3.3V"],["GND",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-black"})}),"GND"],["Signal Out",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-blue"})}),"14"]]}),"\n",(0,s.jsx)(n.h3,{id:"green-led-pin-connections",children:"Green LED Pin Connections"}),"\n",(0,s.jsx)(d.default,{data:[["Green LED","","ESP8266 Dev Board"],["Anode (+)",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-blue"})}),"12"],["Cathode (-)",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-black"})}),"GND"]]}),"\n",(0,s.jsx)(n.h3,{id:"servo-motor-pin-connections",children:"Servo Motor Pin Connections"}),"\n",(0,s.jsx)(d.default,{data:[["Servo Motor","","ESP8266 Dev Board"],["VCC",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-red"})}),"3.3V"],["GND",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-black"})}),"GND"],["PWM",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-blue"})}),"13"]]}),"\n",(0,s.jsx)(n.h2,{id:"circuit-diagram",children:"Circuit Diagram"}),"\n",(0,s.jsx)("img",{src:h,alt:"Circuit Diagram",style:{display:"block",margin:"20px auto"}}),"\n",(0,s.jsx)(n.h2,{id:"link-to-code-explanation-and-tutorial",children:"Link to Code Explanation and Tutorial"}),"\n",(0,s.jsx)(n.p,{children:"For a comprehensive guide on programming this node, including detailed code explanations and step-by-step tutorials, please refer to the following link. This resource provides all necessary information to understand the functioning of the node, enabling users to effectively program and customize it according to their specific requirements."}),"\n",(0,s.jsx)(n.p,{children:(0,s.jsx)(n.a,{href:"/docs/tutorial/node_code_explained",children:"Visit the Node Programming Guide and Code Explanation"})})]})}function f(e={}){const{wrapper:n}={...(0,t.R)(),...e.components};return n?(0,s.jsx)(n,{...e,children:(0,s.jsx)(j,{...e})}):j(e)}},380:(e,n,i)=>{i.r(n),i.d(n,{default:()=>t});i(6540);var s=i(4848);const t=e=>{let{title:n,data:i}=e;return Array.isArray(i)?(0,s.jsx)("table",{className:"tabler connections",children:(0,s.jsx)("tbody",{children:i.map(((e,n)=>(0,s.jsx)("tr",{className:0===n?"tabler-header":"",children:e.map(((e,n)=>(0,s.jsx)("td",{children:e},n)))},n)))})}):(0,s.jsx)("div",{children:"Loading data..."})}},6301:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/files/ESP8266-c7be0444efbfb4a1b209f7e33ff88a49.pdf"},9459:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/files/ESP8266_NodeMCU_Pinout-a8349172ff213e496f1bbc4f5965f4c9.pdf"},3898:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/files/PIR-1b24c83fac9342d273883565ebeca64f.pdf"},7006:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/files/Servo_SG90-203ba23f92e9e281f8d96fe24651a5c9.pdf"},9007:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/files/Yellow_Green_LED-7e3692743d79b1f00d89abd5e7cdb4bd.pdf"},1885:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/images/Green_LED-35aa48fccb9bcc5a93c013b78301e86b.png"},7333:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/images/TOWER_PRO_9g_Servo-ee00a5443837898de3530d40be5166f9.webp"},8453:(e,n,i)=>{i.d(n,{R:()=>o,x:()=>a});var s=i(6540);const t={},r=s.createContext(t);function o(e){const n=s.useContext(r);return s.useMemo((function(){return"function"==typeof e?e(n):{...n,...e}}),[n,e])}function a(e){let n;return n=e.disableParentContext?"function"==typeof e.components?e.components(t):e.components||t:o(e.components),s.createElement(r.Provider,{value:n},e.children)}}}]);