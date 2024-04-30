"use strict";(self.webpackChunkacps_documentation=self.webpackChunkacps_documentation||[]).push([[3073,5258],{3084:(e,n,i)=>{i.r(n),i.d(n,{assets:()=>j,contentTitle:()=>x,default:()=>y,frontMatter:()=>m,metadata:()=>g,toc:()=>f});var s=i(4848),t=i(8453);const r=i.p+"assets/images/Temp-e64c87e7804e9cffae3f3cbb7401c3c2.jpg";var a=i(237),o=i(5358);const l=i.p+"assets/images/LM35-5e24596b640d3b5c514c452eef3da99a.png";var c=i(7966),d=i(5113),h=i(2179),p=i(380);const u=i.p+"assets/images/Temperature-Control-System_Circuit_Diagram_bb-c210604caa90c3113ce13ec187aeea77.png",m={title:"Node 10: Temperature Control System",description:"Manages and regulates environmental temperatures to maintain specific conditions in sensitive areas."},x="Node 10: Temperature Control System",g={id:"physical_components/node10",title:"Node 10: Temperature Control System",description:"Manages and regulates environmental temperatures to maintain specific conditions in sensitive areas.",source:"@site/docs/physical_components/node10.mdx",sourceDirName:"physical_components",slug:"/physical_components/node10",permalink:"/ACPS-Lab/docs/physical_components/node10",draft:!1,unlisted:!1,tags:[],version:"current",frontMatter:{title:"Node 10: Temperature Control System",description:"Manages and regulates environmental temperatures to maintain specific conditions in sensitive areas."},sidebar:"docs",previous:{title:"Node 9: Orientation Visualizer, Free Fall, and Crash Detector",permalink:"/ACPS-Lab/docs/physical_components/node9"},next:{title:"Node 11: Vibration and Shock Detector",permalink:"/ACPS-Lab/docs/physical_components/node11"}},j={},f=[{value:"Components Summary",id:"components-summary",level:2},{value:"Components Overview",id:"components-overview",level:2},{value:"Controller: Arduino Nano 33 IoT",id:"controller-arduino-nano-33-iot",level:3},{value:"Technical Specifications and Features",id:"technical-specifications-and-features",level:4},{value:"References",id:"references",level:4},{value:"Sensor: LM35 Temperature Sensor",id:"sensor-lm35-temperature-sensor",level:3},{value:"Technical Specifications and Features:",id:"technical-specifications-and-features-1",level:4},{value:"References:",id:"references-1",level:4},{value:"Actuator: LED",id:"actuator-led",level:3},{value:"Technical Specifications and Features",id:"technical-specifications-and-features-2",level:4},{value:"References",id:"references-2",level:4},{value:"Actuator: Relay",id:"actuator-relay",level:3},{value:"Technical Specifications and Features",id:"technical-specifications-and-features-3",level:4},{value:"References",id:"references-3",level:4},{value:"Actuator: DC Fan",id:"actuator-dc-fan",level:3},{value:"Technical Specifications and Features:",id:"technical-specifications-and-features-4",level:4},{value:"References:",id:"references-4",level:4},{value:"Communication Protocols",id:"communication-protocols",level:2},{value:"Controller Communication",id:"controller-communication",level:3},{value:"Wi-Fi Connection:",id:"wi-fi-connection",level:4},{value:"MQTT Protocol:",id:"mqtt-protocol",level:4},{value:"Sensor Communication",id:"sensor-communication",level:3},{value:"Analog Data Communication (LM35 Temperature Sensor):",id:"analog-data-communication-lm35-temperature-sensor",level:4},{value:"Pin Connections",id:"pin-connections",level:2},{value:"LM35 Temperature Sensor Pin Connections",id:"lm35-temperature-sensor-pin-connections",level:3},{value:"Red LED Pin Connections",id:"red-led-pin-connections",level:3},{value:"Fan Relay Module Pin Connections",id:"fan-relay-module-pin-connections",level:3},{value:"Circuit Diagram",id:"circuit-diagram",level:2},{value:"Link to Code Explanation and Tutorial",id:"link-to-code-explanation-and-tutorial",level:2}];function v(e){const n={a:"a",h1:"h1",h2:"h2",h3:"h3",h4:"h4",li:"li",p:"p",span:"span",strong:"strong",ul:"ul",...(0,t.R)(),...e.components};return(0,s.jsxs)(s.Fragment,{children:[(0,s.jsx)(n.h1,{id:"node-10-temperature-control-system",children:"Node 10: Temperature Control System"}),"\n",(0,s.jsx)(n.p,{children:"The Temperature Control System, employs the Arduino Nano 33 IoT alongside the LM35 temperature sensor to manage and regulate temperature in controlled environments such as greenhouses or server rooms. This node optimizes conditions through automated adjustments, ensuring environmental stability which is crucial for sensitive operations and processes."}),"\n",(0,s.jsx)("img",{src:r,alt:"Temperature Control System",style:{width:"75%",display:"block",margin:"20px auto"}}),"\n",(0,s.jsx)(n.h2,{id:"components-summary",children:"Components Summary"}),"\n",(0,s.jsx)("div",{style:{display:"flex",justifyContent:"center",margin:"20px"},children:(0,s.jsxs)("table",{style:{width:"75%",borderCollapse:"collapse"},children:[(0,s.jsx)("thead",{children:(0,s.jsxs)("tr",{children:[(0,s.jsx)("th",{style:{padding:"8px",width:"30%",textAlign:"center"},children:"Component Type"}),(0,s.jsx)("th",{style:{padding:"8px",width:"70%",textAlign:"center"},children:"Component Name"})]})}),(0,s.jsxs)("tbody",{children:[(0,s.jsxs)("tr",{children:[(0,s.jsx)("td",{style:{padding:"8px",textAlign:"right"},children:"Controller"}),(0,s.jsx)("td",{style:{padding:"8px"},children:(0,s.jsx)("a",{href:"#controller-arduino-nano-33-iot",children:"Arduino Nano 33 IoT"})})]}),(0,s.jsxs)("tr",{children:[(0,s.jsx)("td",{style:{padding:"8px",textAlign:"right"},children:"Sensor"}),(0,s.jsx)("td",{style:{padding:"8px"},children:(0,s.jsx)("a",{href:"#sensor-lm35-temperature-sensor",children:"LM35 Temperature Sensor"})})]}),(0,s.jsxs)("tr",{children:[(0,s.jsx)("td",{style:{padding:"8px",textAlign:"right"},children:"Actuator"}),(0,s.jsx)("td",{style:{padding:"8px"},children:(0,s.jsx)("a",{href:"#actuator-led",children:"Red LED"})})]}),(0,s.jsxs)("tr",{children:[(0,s.jsx)("td",{}),(0,s.jsx)("td",{style:{padding:"8px"},children:(0,s.jsx)("a",{href:"#actuator-relay",children:"Relay"})})]}),(0,s.jsxs)("tr",{children:[(0,s.jsx)("td",{}),(0,s.jsx)("td",{style:{padding:"8px"},children:(0,s.jsx)("a",{href:"#actuator-dc-fan",children:"5V DC Fan"})})]})]})]})}),"\n",(0,s.jsx)(n.h2,{id:"components-overview",children:"Components Overview"}),"\n",(0,s.jsx)(n.h3,{id:"controller-arduino-nano-33-iot",children:"Controller: Arduino Nano 33 IoT"}),"\n",(0,s.jsx)(n.p,{children:"The Arduino Nano 33 IoT is a compact and versatile development board designed for Internet of Things (IoT) applications. It features built-in Wi-Fi and Bluetooth connectivity, making it easy to create IoT devices that can communicate wirelessly. The board is power-efficient, making it suitable for battery-powered applications. With compatibility with the Arduino IDE, it simplifies the development process for IoT projects like remote monitoring, home automation, and sensor networks. Additionally, it supports secure communication with hardware-based encryption, making it suitable for applications requiring data security. Its small size and extensive capabilities make it a popular choice for IoT enthusiasts and developers looking to create connected and innovative devices."}),"\n",(0,s.jsx)("img",{src:a.A,alt:"Arduino Nano 33 IoT",style:{width:"75%",display:"block",margin:"20px auto"}}),"\n",(0,s.jsx)("img",{src:o.A,alt:"Arduino Nano 33 IoT",style:{width:"75%",display:"block",margin:"20px auto"}}),"\n",(0,s.jsx)(n.h4,{id:"technical-specifications-and-features",children:"Technical Specifications and Features"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Microcontroller"}),": The Arduino Nano 33 IoT is equipped with the Microchip ATSAMW25, featuring a 32-bit ARM Cortex-M0+ processor."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Operating Voltage"}),": 3.3V"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Input Voltage (Recommended)"}),": 5V via USB or an external Li-Po battery."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Digital I/O Pins"}),": 8, including PWM support."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Analog Input Pins"}),": 8"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"UART Ports"}),": 1 for serial communication."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"SPI Interface"}),": 1 for communication with other devices."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"I2C Interface"}),": 1 for inter-device communication."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Flash Memory"}),": 256KB, with 6KB used by the bootloader."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"SRAM"}),": 32KB"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Clock Speed"}),": 48MHz"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Wireless Connectivity"}),": Wi-Fi via the u-blox NINAW102 module."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Crypto chip"}),": Included for secure communication."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"USB Port"}),": Micro USB for programming and power."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Li-Po Battery Connector"}),": JST-PH2 connector for connecting a Li-Po battery."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Charging Current"}),": 450mA"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Operating Temperature"}),": -40\xb0C to +85\xb0C"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Dimensions"}),": Approximately 45mm x 18.5mm"]}),"\n"]}),"\n",(0,s.jsx)(n.h4,{id:"references",children:"References"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.a,{target:"_blank","data-noBrokenLinkCheck":!0,href:i(3624).A+"",children:"Arduino Nano 33 IoT Datasheet"})}),"\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.a,{target:"_blank","data-noBrokenLinkCheck":!0,href:i(72).A+"",children:"Arduino Nano 33 IoT Pinout"})}),"\n"]}),"\n",(0,s.jsx)(n.h3,{id:"sensor-lm35-temperature-sensor",children:"Sensor: LM35 Temperature Sensor"}),"\n",(0,s.jsx)(n.p,{children:"The LM35 is a widely used analog temperature sensor that provides a straightforward and accurate way to measure temperature in various electronic applications. It operates on the principle that its output voltage increases linearly with temperature, with a slope of 10 mV per degree Celsius. This means that by measuring the LM35's output voltage, you can directly determine the temperature it's exposed to. The LM35 is known for its simplicity, low cost, and high precision, making it a popular choice for temperature monitoring in devices like thermostats, weather stations, and temperature-controlled systems. Its ease of integration, wide temperature range, and minimal calibration requirements make it a go-to sensor for temperature sensing needs in both hobbyist and industrial projects."}),"\n",(0,s.jsx)("img",{src:l,alt:"LM35 Temperature Sensor",style:{width:"60%",display:"block",margin:"20px auto"}}),"\n",(0,s.jsx)(n.h4,{id:"technical-specifications-and-features-1",children:"Technical Specifications and Features:"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.strong,{children:"Calibrated Directly in Celsius (Centigrade)"})}),"\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.strong,{children:"Linear +10-mV/\xb0C Scale Factor"})}),"\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.strong,{children:"0.5\xb0C Ensured Accuracy (at 25\xb0C)"})}),"\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.strong,{children:"Rated for Full \u221255\xb0C to 150\xb0C Range"})}),"\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.strong,{children:"Suitable for Remote Applications"})}),"\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.strong,{children:"Low-Cost Due to Wafer-Level Trimming"})}),"\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.strong,{children:"Operates From 4V to 30V"})}),"\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.strong,{children:"Less Than 60-\u03bcA Current Drain"})}),"\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.strong,{children:"Low Self-Heating, 0.08\xb0C in Still Air"})}),"\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.strong,{children:"Non-Linearity Only \xb1\xbc\xb0C Typical"})}),"\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.strong,{children:"Low-Impedance Output, 0.1\u03a9 for 1-mA Load"})}),"\n"]}),"\n",(0,s.jsx)(n.h4,{id:"references-1",children:"References:"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.a,{target:"_blank","data-noBrokenLinkCheck":!0,href:i(4548).A+"",children:"LM35 Datasheet"})}),"\n"]}),"\n",(0,s.jsx)(n.h3,{id:"actuator-led",children:"Actuator: LED"}),"\n",(0,s.jsx)(n.p,{children:"LEDs, or Light Emitting Diodes, are semiconductor devices that emit light when an electric current is applied. Known for their energy efficiency, compact size, and long life, LEDs are extensively used across various applications including electronic devices, display panels, indicators, and general lighting. Their ability to be manufactured in various colors and sizes makes LEDs exceptionally versatile for design and functional purposes in numerous projects."}),"\n",(0,s.jsx)("img",{src:c.A,alt:"Red LED",style:{width:"25%",display:"block",margin:"20px auto"}}),"\n",(0,s.jsx)(n.h4,{id:"technical-specifications-and-features-2",children:"Technical Specifications and Features"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Dice Material"}),": Gallium Arsenide Phosphide on Gallium Phosphide (GaAsP on GaP/GaP)."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Emitted Colors"}),": Available in Green and Red."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Lens Color"}),": Green Diffused and Red Diffused."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Peak Wavelength"}),":","\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsx)(n.li,{children:"Green: 568 nm"}),"\n",(0,s.jsx)(n.li,{children:"Red: 587 nm"}),"\n"]}),"\n"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Viewing Angle"}),": 45 degrees, offering focused illumination."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Luminous Intensity (IV)"}),":","\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsx)(n.li,{children:"Green: 15 mcd"}),"\n",(0,s.jsx)(n.li,{children:"Red: 14 mcd"}),"\n"]}),"\n"]}),"\n"]}),"\n",(0,s.jsx)(n.h4,{id:"references-2",children:"References"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.a,{target:"_blank","data-noBrokenLinkCheck":!0,href:i(9192).A+"",children:"Red LED Datasheet"})}),"\n"]}),"\n",(0,s.jsx)(n.h3,{id:"actuator-relay",children:"Actuator: Relay"}),"\n",(0,s.jsx)("img",{src:d.A,alt:"Relay",style:{width:"60%",display:"block",margin:"20px auto"}}),"\n",(0,s.jsx)(n.p,{children:"The 3V relay module is an electronic switch that operates at a low voltage of 3 volts, making it suitable for use in low-power circuits and microcontroller-based projects. It typically consists of a relay, driver circuitry, and input/output connectors. The relay serves as an electromechanical switch, capable of controlling higher voltage and current loads with a low-voltage signal. This module is commonly used to interface microcontrollers, Arduino boards, or other digital devices with appliances, lights, or other electrical loads. It simplifies the process of controlling high-power devices by allowing you to use a low-voltage signal to open or close the relay contacts, thereby turning the connected load on or off. The 3V relay module finds applications in home automation, robotics, and various electronic projects where remote or automated control of electrical devices is needed."}),"\n",(0,s.jsx)(n.h4,{id:"technical-specifications-and-features-3",children:"Technical Specifications and Features"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Operate Voltage"}),": 3.3V-5V"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Input Current"}),": 100mA"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Rated Load"}),": 5A@250VAC, 5A@30VDC"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Contact Resistance"}),": 50m\u03a9 @6VDC 1A"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Insulation Resistance"}),": 100M\u03a9 10ms Max."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Operation Time"}),": 10ms Max."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Release Time"}),": 5ms Max."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Input Interface"}),": Digital"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Dimensions"}),": 42mm x 24mm x 18.5mm"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Weight"}),": G.W 19g"]}),"\n"]}),"\n",(0,s.jsx)(n.h4,{id:"references-3",children:"References"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.a,{target:"_blank","data-noBrokenLinkCheck":!0,href:i(1672).A+"",children:"Relay Datasheet"})}),"\n"]}),"\n",(0,s.jsx)(n.h3,{id:"actuator-dc-fan",children:"Actuator: DC Fan"}),"\n",(0,s.jsx)(n.p,{children:"A 5V DC fan is a compact and efficient cooling device powered by a 5-volt direct current (DC) source. It typically features a brushless motor design for quiet operation and longevity. These fans come in various sizes, including common dimensions like 80mm and 120mm, and are equipped with blades that circulate air to dissipate heat from electronic components or provide ventilation in various applications. Due to their low voltage requirements, 5V DC fans are often used in small electronics, such as laptops, Raspberry Pi projects, and electronic enclosures, to prevent overheating and maintain proper thermal management. They are also favored in portable and low-power setups, where they help maintain optimal operating temperatures without draining excessive power."}),"\n",(0,s.jsx)("img",{src:h.A,alt:"DC Fan",style:{width:"50%",display:"block",margin:"20px auto"}}),"\n",(0,s.jsx)(n.h4,{id:"technical-specifications-and-features-4",children:"Technical Specifications and Features:"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Operating Voltage:"})," 5V"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Current:"})," 0.2 A"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Type:"})," Brushless DC fan"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Fan Dimensions:"})," 30mm x 30mm x 8mm"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Wire Length:"}),' 3.25" / 80mm']}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Weight:"})," 6.2g / 0.22oz"]}),"\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.strong,{children:"Mounting Screws & Nuts Included"})}),"\n"]}),"\n",(0,s.jsx)(n.h4,{id:"references-4",children:"References:"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.a,{target:"_blank","data-noBrokenLinkCheck":!0,href:i(6936).A+"",children:"DC Cooling Fan Datasheet"})}),"\n"]}),"\n",(0,s.jsx)(n.h2,{id:"communication-protocols",children:"Communication Protocols"}),"\n",(0,s.jsx)(n.h3,{id:"controller-communication",children:"Controller Communication"}),"\n",(0,s.jsx)(n.p,{children:"The communication between the Arduino Nano 33 IoT controller and the ThingsBoard server involves two key protocols: Wi-Fi and MQTT."}),"\n",(0,s.jsx)(n.h4,{id:"wi-fi-connection",children:"Wi-Fi Connection:"}),"\n",(0,s.jsx)(n.p,{children:"The Arduino Nano 33 IoT connects to a local Wi-Fi network, enabling it to access the internet. This connection provides the foundation for communication with external services like the ThingsBoard server."}),"\n",(0,s.jsx)(n.h4,{id:"mqtt-protocol",children:"MQTT Protocol:"}),"\n",(0,s.jsx)(n.p,{children:"The Message Queuing Telemetry Transport (MQTT) protocol is employed for transmitting data between the controller and the ThingsBoard server. MQTT is a lightweight and efficient protocol designed for remote devices with limited resources. It facilitates the publication of data (such as access events) from the controller to the server, as well as the subscription to commands or acknowledgements from the server. MQTT ensures secure and reliable communication, making it ideal for real-time data visualization and monitoring on the ThingsBoard dashboard. The controller publishes messages related to access events, which are then visualized on the dashboard, offering insights into the system's performance and access patterns."}),"\n",(0,s.jsx)(n.h3,{id:"sensor-communication",children:"Sensor Communication"}),"\n",(0,s.jsx)(n.p,{children:"The primary sensor used in this node is the LM35 temperature sensor, which communicates with the Arduino Nano 33 IoT controller through analog data."}),"\n",(0,s.jsx)(n.h4,{id:"analog-data-communication-lm35-temperature-sensor",children:"Analog Data Communication (LM35 Temperature Sensor):"}),"\n",(0,s.jsx)(n.p,{children:"The LM35 temperature sensor employs analog data communication to relay precise temperature measurements to the Arduino Nano 33 IoT controller. The sensor generates an analog voltage signal that is directly proportional to the ambient temperature. The controller, equipped with analog-to-digital conversion capabilities, reads this analog signal and interprets it as temperature data. This communication method allows for high precision in temperature measurement and control. When the temperature exceeds predefined thresholds, the controller triggers the relay to activate or deactivate the connected fan, thereby maintaining the desired environmental conditions."}),"\n",(0,s.jsx)(n.h2,{id:"pin-connections",children:"Pin Connections"}),"\n",(0,s.jsx)(n.p,{children:"This section provides details about the pin connections between the components used in the node and the Arduino Nano 33 IoT controller."}),"\n",(0,s.jsx)(n.h3,{id:"lm35-temperature-sensor-pin-connections",children:"LM35 Temperature Sensor Pin Connections"}),"\n",(0,s.jsx)(p.default,{data:[["LM35 Temperature Sensor","","Arduino Nano 33 IoT"],["VCC",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-red"})}),"3.3V"],["GND",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-black"})}),"GND"],["Signal Out",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-blue"})}),"A1"]]}),"\n",(0,s.jsx)(n.h3,{id:"red-led-pin-connections",children:"Red LED Pin Connections"}),"\n",(0,s.jsx)(p.default,{data:[["Red LED","","Arduino Nano 33 IoT"],["Anode (+)",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-blue"})}),"7"],["Cathode (-)",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-black"})}),"GND"]]}),"\n",(0,s.jsx)(n.h3,{id:"fan-relay-module-pin-connections",children:"Fan Relay Module Pin Connections"}),"\n",(0,s.jsx)(p.default,{data:[["Fan Relay","","ESP32 Dev Board"],["VCC",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-red"})}),"3.3V"],["GND",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-black"})}),"GND"],["Signal In",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-blue"})}),"8"]]}),"\n",(0,s.jsx)(n.h2,{id:"circuit-diagram",children:"Circuit Diagram"}),"\n",(0,s.jsx)("img",{src:u,alt:"Circuit Diagram",style:{display:"block",margin:"20px auto"}}),"\n",(0,s.jsx)(n.h2,{id:"link-to-code-explanation-and-tutorial",children:"Link to Code Explanation and Tutorial"}),"\n",(0,s.jsx)(n.p,{children:"For a comprehensive guide on programming this node, including detailed code explanations and step-by-step tutorials, please refer to the following link. This resource provides all necessary information to understand the functioning of the node, enabling users to effectively program and customize it according to their specific requirements."}),"\n",(0,s.jsx)(n.p,{children:(0,s.jsx)(n.a,{href:"/ACPS-Lab/docs/tutorial/node_code_explained",children:"Visit the Node Programming Guide and Code Explanation"})})]})}function y(e={}){const{wrapper:n}={...(0,t.R)(),...e.components};return n?(0,s.jsx)(n,{...e,children:(0,s.jsx)(v,{...e})}):v(e)}},380:(e,n,i)=>{i.r(n),i.d(n,{default:()=>t});i(6540);var s=i(4848);const t=e=>{let{title:n,data:i}=e;return Array.isArray(i)?(0,s.jsx)("table",{className:"tabler connections",children:(0,s.jsx)("tbody",{children:i.map(((e,n)=>(0,s.jsx)("tr",{className:0===n?"tabler-header":"",children:e.map(((e,n)=>(0,s.jsx)("td",{children:e},n)))},n)))})}):(0,s.jsx)("div",{children:"Loading data..."})}},3624:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/files/Arduino_Nano_33_IoT-4e1a0f88dada4b7447e81712d4939b60.pdf"},72:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/files/Arduino_Nano_33_IoT_Pinout-4cbfbb72283976469dc09308e7f19d58.pdf"},6936:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/files/Cooling_Fan-365f1889e77380df45de62226693558a.pdf"},9192:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/files/Red_LED-a072d01b3605265772212d6fe2dd41d1.pdf"},1672:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/files/Relay-97e6edd3157ac2061b965e333f553b8f.pdf"},4548:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/files/lm35-7e8a602d5bed0169d7346d3a43c593ed.pdf"},237:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/images/Arduino_Nano_33_IoT-c0c8174db8935a13cc24fd7ed9e9c369.png"},5358:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/images/Arduino_Nano_33_IoT_2-45034393adf410ac1d29943b6e4c8925.png"},2179:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/images/Cooling-Fan-7f7658e48a32a38811caae5e8b530c4e.png"},7966:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/images/Red_LED-d92bcb72fcb8adf5910c9030d12fa836.webp"},5113:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/images/Relay-90bb8025ab746e59812be8c9e83a278f.jpg"},8453:(e,n,i)=>{i.d(n,{R:()=>a,x:()=>o});var s=i(6540);const t={},r=s.createContext(t);function a(e){const n=s.useContext(r);return s.useMemo((function(){return"function"==typeof e?e(n):{...n,...e}}),[n,e])}function o(e){let n;return n=e.disableParentContext?"function"==typeof e.components?e.components(t):e.components||t:a(e.components),s.createElement(r.Provider,{value:n},e.children)}}}]);