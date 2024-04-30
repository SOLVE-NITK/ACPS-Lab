"use strict";(self.webpackChunkacps_documentation=self.webpackChunkacps_documentation||[]).push([[4143,5258],{5086:(e,n,i)=>{i.r(n),i.d(n,{assets:()=>j,contentTitle:()=>x,default:()=>y,frontMatter:()=>m,metadata:()=>g,toc:()=>f});var s=i(4848),r=i(8453);const t=i.p+"assets/images/weather-310e09a01ac26a2a96c53a7126e7476d.jpg";var a=i(237),o=i(5358);const l=i.p+"assets/images/BMP_280-e147b7984910bac9cc9245e8aaf6fefc.png",c=i.p+"assets/images/AM2301-afd505aeecc5fbcaaab7685c73b00f24.jpg",d=i.p+"assets/images/DS3231-0ad331eab159b8429752f35e55e2e79b.jpg",h=i.p+"assets/images/OLED_0.96-ab7a1225c82dc1277d29a8d5b1c5f91a.png";var u=i(380);const p=i.p+"assets/images/Local-Weather-Monitoring-System__Circuit_Diagram_bb-b5856b324febc22e4b7c3d8b48f81218.png",m={title:"Node 6: Local Weather Station",description:"Collects and displays local weather data, aiding in community weather tracking and environmental education."},x="Node 6: Local Weather Station",g={id:"physical_components/node6",title:"Node 6: Local Weather Station",description:"Collects and displays local weather data, aiding in community weather tracking and environmental education.",source:"@site/docs/physical_components/node6.mdx",sourceDirName:"physical_components",slug:"/physical_components/node6",permalink:"/ACPS-Lab/docs/physical_components/node6",draft:!1,unlisted:!1,tags:[],version:"current",frontMatter:{title:"Node 6: Local Weather Station",description:"Collects and displays local weather data, aiding in community weather tracking and environmental education."},sidebar:"docs",previous:{title:"Node 5: Light Intensity, Loud Noise and Color Detector",permalink:"/ACPS-Lab/docs/physical_components/node5"},next:{title:"Node 7: Motion Detector",permalink:"/ACPS-Lab/docs/physical_components/node7"}},j={},f=[{value:"Components Summary",id:"components-summary",level:2},{value:"Components Overview",id:"components-overview",level:2},{value:"Controller: Arduino Nano 33 IoT",id:"controller-arduino-nano-33-iot",level:3},{value:"Technical Specifications and Features",id:"technical-specifications-and-features",level:4},{value:"References",id:"references",level:4},{value:"Sensor: BMP280 Pressure and Altitude Sensor",id:"sensor-bmp280-pressure-and-altitude-sensor",level:3},{value:"Technical Specifications and Features",id:"technical-specifications-and-features-1",level:4},{value:"References",id:"references-1",level:4},{value:"Sensor: AM2301 Capacitive Digital Temperature &amp; Humidity Sensor",id:"sensor-am2301-capacitive-digital-temperature--humidity-sensor",level:3},{value:"Technical Specifications and Features",id:"technical-specifications-and-features-2",level:4},{value:"References",id:"references-2",level:4},{value:"Sensor: DS3231 Precision RTC Module",id:"sensor-ds3231-precision-rtc-module",level:3},{value:"Technical Specifications and Features:",id:"technical-specifications-and-features-3",level:4},{value:"References:",id:"references-3",level:4},{value:"Actuator: 0.96\u201d 128\xd764 OLED Display Module",id:"actuator-096-12864-oled-display-module",level:3},{value:"Technical Specifications and Features:",id:"technical-specifications-and-features-4",level:4},{value:"References:",id:"references-4",level:4},{value:"Communication Protocols",id:"communication-protocols",level:2},{value:"Controller Communication",id:"controller-communication",level:3},{value:"Wi-Fi Connection",id:"wi-fi-connection",level:4},{value:"MQTT Protocol",id:"mqtt-protocol",level:4},{value:"Sensor Communication",id:"sensor-communication",level:3},{value:"I2C Protocol (for Sensors)",id:"i2c-protocol-for-sensors",level:4},{value:"Actuator Communication",id:"actuator-communication",level:3},{value:"I2C Protocol (for Actuator - OLED Display)",id:"i2c-protocol-for-actuator---oled-display",level:4},{value:"Pin Connections",id:"pin-connections",level:2},{value:"BMP280 Module Pin Connections",id:"bmp280-module-pin-connections",level:3},{value:"AM2301 Module Pin Connections",id:"am2301-module-pin-connections",level:3},{value:"DS3231 Module Pin Connections",id:"ds3231-module-pin-connections",level:3},{value:"0.96\u201d 128\xd764 OLED Module Pin Connections",id:"096-12864-oled-module-pin-connections",level:3},{value:"Circuit Diagram",id:"circuit-diagram",level:2},{value:"Link to Code Explanation and Tutorial",id:"link-to-code-explanation-and-tutorial",level:2}];function v(e){const n={a:"a",h1:"h1",h2:"h2",h3:"h3",h4:"h4",li:"li",p:"p",span:"span",strong:"strong",ul:"ul",...(0,r.R)(),...e.components};return(0,s.jsxs)(s.Fragment,{children:[(0,s.jsx)(n.h1,{id:"node-6-local-weather-station",children:"Node 6: Local Weather Station"}),"\n",(0,s.jsx)(n.p,{children:"The Local Weather Station,  features an Arduino Nano 33 IoT that collects comprehensive meteorological data including temperature, humidity, and atmospheric pressure. Positioned as a Local Weather Station, this node is integral for community weather tracking, agricultural planning, and educational purposes, providing valuable real-time weather information through a user-friendly IoT interface."}),"\n",(0,s.jsx)("img",{src:t,alt:"Local Weather Station",style:{width:"75%",display:"block",margin:"20px auto"}}),"\n",(0,s.jsx)(n.h2,{id:"components-summary",children:"Components Summary"}),"\n",(0,s.jsx)("div",{style:{display:"flex",justifyContent:"center",margin:"20px"},children:(0,s.jsxs)("table",{style:{width:"75%",borderCollapse:"collapse"},children:[(0,s.jsx)("thead",{children:(0,s.jsxs)("tr",{children:[(0,s.jsx)("th",{style:{padding:"8px",width:"30%",textAlign:"center"},children:"Component Type"}),(0,s.jsx)("th",{style:{padding:"8px",width:"70%",textAlign:"center"},children:"Component Name"})]})}),(0,s.jsxs)("tbody",{children:[(0,s.jsxs)("tr",{children:[(0,s.jsx)("td",{style:{padding:"8px",textAlign:"right"},children:"Controller"}),(0,s.jsx)("td",{style:{padding:"8px"},children:(0,s.jsx)("a",{href:"#controller-arduino-nano-33-iot",children:"Arduino Nano 33 IoT"})})]}),(0,s.jsxs)("tr",{children:[(0,s.jsx)("td",{style:{padding:"8px",textAlign:"right"},children:"Sensor"}),(0,s.jsx)("td",{style:{padding:"8px"},children:(0,s.jsx)("a",{href:"#sensor-bmp280-pressure-and-altitude-sensor",children:"Pressure and Altitude Sensor, BMP280"})})]}),(0,s.jsxs)("tr",{children:[(0,s.jsx)("td",{style:{padding:"8px",textAlign:"right"}}),(0,s.jsx)("td",{style:{padding:"8px"},children:(0,s.jsx)("a",{href:"#sensor-am2301-capacitive-digital-temperature--humidity-sensor",children:"Capacitive Digital Temperature & Humidity Sensor, AM2301"})})]}),(0,s.jsxs)("tr",{children:[(0,s.jsx)("td",{style:{padding:"8px",textAlign:"right"}}),(0,s.jsx)("td",{style:{padding:"8px"},children:(0,s.jsx)("a",{href:"#sensor-ds3231-precision-rtc-module",children:"DS3231 Precision RTC Module"})})]}),(0,s.jsxs)("tr",{children:[(0,s.jsx)("td",{style:{padding:"8px",textAlign:"right"},children:"Actuator"}),(0,s.jsx)("td",{style:{padding:"8px"},children:(0,s.jsx)("a",{href:"#actuator-096-12864-oled-display-module",children:'0.96" 128x64 OLED Display Module'})})]})]})]})}),"\n",(0,s.jsx)(n.h2,{id:"components-overview",children:"Components Overview"}),"\n",(0,s.jsx)(n.h3,{id:"controller-arduino-nano-33-iot",children:"Controller: Arduino Nano 33 IoT"}),"\n",(0,s.jsx)(n.p,{children:"The Arduino Nano 33 IoT is a compact and versatile development board designed for Internet of Things (IoT) applications. It features built-in Wi-Fi and Bluetooth connectivity, making it easy to create IoT devices that can communicate wirelessly. The board is power-efficient, making it suitable for battery-powered applications. With compatibility with the Arduino IDE, it simplifies the development process for IoT projects like remote monitoring, home automation, and sensor networks. Additionally, it supports secure communication with hardware-based encryption, making it suitable for applications requiring data security. Its small size and extensive capabilities make it a popular choice for IoT enthusiasts and developers looking to create connected and innovative devices."}),"\n",(0,s.jsx)("img",{src:a.A,alt:"Arduino Nano 33 IoT",style:{width:"75%",display:"block",margin:"20px auto"}}),"\n",(0,s.jsx)("img",{src:o.A,alt:"Arduino Nano 33 IoT",style:{width:"75%",display:"block",margin:"20px auto"}}),"\n",(0,s.jsx)(n.h4,{id:"technical-specifications-and-features",children:"Technical Specifications and Features"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Microcontroller"}),": The Arduino Nano 33 IoT is equipped with the Microchip ATSAMW25, featuring a 32-bit ARM Cortex-M0+ processor."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Operating Voltage"}),": 3.3V"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Input Voltage (Recommended)"}),": 5V via USB or an external Li-Po battery."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Digital I/O Pins"}),": 8, including PWM support."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Analog Input Pins"}),": 8"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"UART Ports"}),": 1 for serial communication."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"SPI Interface"}),": 1 for communication with other devices."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"I2C Interface"}),": 1 for inter-device communication."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Flash Memory"}),": 256KB, with 6KB used by the bootloader."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"SRAM"}),": 32KB"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Clock Speed"}),": 48MHz"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Wireless Connectivity"}),": Wi-Fi via the u-blox NINAW102 module."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Crypto chip"}),": Included for secure communication."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"USB Port"}),": Micro USB for programming and power."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Li-Po Battery Connector"}),": JST-PH2 connector for connecting a Li-Po battery."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Charging Current"}),": 450mA"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Operating Temperature"}),": -40\xb0C to +85\xb0C"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Dimensions"}),": Approximately 45mm x 18.5mm"]}),"\n"]}),"\n",(0,s.jsx)(n.h4,{id:"references",children:"References"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.a,{target:"_blank","data-noBrokenLinkCheck":!0,href:i(3624).A+"",children:"Arduino Nano 33 IoT Datasheet"})}),"\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.a,{target:"_blank","data-noBrokenLinkCheck":!0,href:i(72).A+"",children:"Arduino Nano 33 IoT Pinout"})}),"\n"]}),"\n",(0,s.jsx)(n.h3,{id:"sensor-bmp280-pressure-and-altitude-sensor",children:"Sensor: BMP280 Pressure and Altitude Sensor"}),"\n",(0,s.jsx)(n.p,{children:"The BMP280 is a versatile Pressure and Altitude Sensor commonly used in various applications, including weather stations, drones, and altitude tracking devices. This sensor is designed to precisely measure atmospheric pressure and temperature, allowing for accurate altitude calculations. Its ability to provide both pressure and temperature data with high resolution and low power consumption makes it a popular choice for battery-powered and portable projects. The BMP280 employs advanced MEMS (Micro-Electro-Mechanical Systems) technology, ensuring excellent precision and stability over a wide range of environmental conditions."}),"\n",(0,s.jsx)("img",{src:l,alt:"BMP280 Pressure and Altitude Sensor",style:{width:"75%",display:"block",margin:"20px auto"}}),"\n",(0,s.jsx)(n.h4,{id:"technical-specifications-and-features-1",children:"Technical Specifications and Features"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Operating Voltage"}),": The BMP280 operates at a supply voltage between 1.71V and 3.6V."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Pressure Sensor"}),": It measures atmospheric pressure with a range from 300 hPa to 1100 hPa (hectopascals)."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Temperature Sensor"}),": The sensor provides temperature measurement with a range from -40\xb0C to +85\xb0C."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Pressure Resolution"}),": It offers high-resolution pressure data with an accuracy of \xb11 hPa (or \xb18 Pa)."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Temperature Resolution"}),": The temperature accuracy is \xb10.5\xb0C."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Interface"}),": The sensor communicates via I2C and SPI interfaces, providing flexibility for integration with various microcontrollers and devices."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Low Power"}),": It features low power consumption, making it suitable for battery-powered applications."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Calibration Coefficients"}),": The BMP280 includes built-in calibration coefficients for accurate readings."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Sampling Rate"}),": It can provide measurements at different data rates, ranging from ultra-low power to high performance."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Altitude Calculation"}),": With its pressure and temperature data, the BMP280 can calculate altitude accurately."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Tiny Package"}),": The sensor comes in a small 8-pin LGA package, measuring 2.5mm x 2.5mm x 0.93mm."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Digital Filtering"}),": It includes internal digital filtering to improve measurement accuracy."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Operating Modes"}),": The sensor offers different power modes, including sleep mode for minimal power consumption."]}),"\n"]}),"\n",(0,s.jsx)(n.h4,{id:"references-1",children:"References"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.a,{target:"_blank","data-noBrokenLinkCheck":!0,href:i(2796).A+"",children:"BMP280 Datasheet"})}),"\n"]}),"\n",(0,s.jsx)(n.h3,{id:"sensor-am2301-capacitive-digital-temperature--humidity-sensor",children:"Sensor: AM2301 Capacitive Digital Temperature & Humidity Sensor"}),"\n",(0,s.jsx)(n.p,{children:"The AM2301, also known as the DHT21, is a versatile humidity and temperature sensor module. It incorporates a capacitive humidity sensor and a high-precision temperature sensor, allowing it to provide accurate and simultaneous readings of relative humidity and temperature. With a digital output signal, it is easy to interface with microcontrollers and data-logging systems."}),"\n",(0,s.jsx)("img",{src:c,alt:"AM2301 Module",style:{width:"75%",display:"block",margin:"20px auto"}}),"\n",(0,s.jsx)(n.h4,{id:"technical-specifications-and-features-2",children:"Technical Specifications and Features"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Input Voltage"}),": 3.5-5.5V"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Measuring Current"}),": 1-1.5mA"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Humidity Range"}),": 0-100% RH"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Temperature Range"}),": -40\xb0C to 80\xb0C"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Humidity Accuracy"}),": \xb13% RH"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Temperature Accuracy"}),": \xb10.5\xb0C"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Wire Length"}),": 25cm"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Dimensions"}),": 59mm x 27mm x 13mm"]}),"\n"]}),"\n",(0,s.jsx)(n.h4,{id:"references-2",children:"References"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.a,{target:"_blank","data-noBrokenLinkCheck":!0,href:i(8559).A+"",children:"AM2301 Datasheet"})}),"\n"]}),"\n",(0,s.jsx)(n.h3,{id:"sensor-ds3231-precision-rtc-module",children:"Sensor: DS3231 Precision RTC Module"}),"\n",(0,s.jsx)(n.p,{children:"The DS3231 is a highly accurate real-time clock (RTC) module commonly used in electronic devices and microcontroller-based projects. It features a built-in temperature-compensated crystal oscillator (TCXO), which ensures precise timekeeping with minimal drift over extended periods. The DS3231 provides not only time and date information but also a range of alarm functions, making it versatile for various applications."}),"\n",(0,s.jsx)("img",{src:d,alt:"DS3231 Precision RTC Module",style:{width:"75%",display:"block",margin:"20px auto"}}),"\n",(0,s.jsx)(n.h4,{id:"technical-specifications-and-features-3",children:"Technical Specifications and Features:"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Highly Accurate RTC"}),": The DS3231 completely manages all timekeeping functions, offering high accuracy."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Timekeeping Functions"}),": Tracks seconds, minutes, hours, date of the month, month, day of the week, and year, with leap-year compensation valid up to the year 2100."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Temperature Accuracy"}),": \xb12ppm from 0\xb0C to +40\xb0C and \xb13.5ppm from -40\xb0C to +85\xb0C."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Digital Temperature Sensor"}),": Includes a digital temperature sensor with \xb13\xb0C accuracy."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Aging Trim Register"}),": Features a register for ageing trim to fine-tune timekeeping accuracy."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"RST Output/Pushbutton Reset Debounce Input"}),": Offers an RST (reset) output and debounce input for pushbutton reset functionality."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Two Time-of-Day Alarms"}),": Supports two programmable time-of-day alarms."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Programmable Square-Wave Output Signal"}),": Can generate a programmable square-wave output signal."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Simple Serial Interface"}),": Connects easily to most microcontrollers through a fast (400kHz) I2C interface."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Battery-Backup Input"}),": Ensures continuous timekeeping during power interruptions."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Low Power Operation"}),": Operates at low power, extending battery backup run time."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"3.3V Operation"}),": The DS3231 operates at 3.3V."]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Operating Temperature Ranges"}),": Available in Commercial (0\xb0C to +70\xb0C) and Industrial (-40\xb0C to +85\xb0C) ranges."]}),"\n"]}),"\n",(0,s.jsx)(n.h4,{id:"references-3",children:"References:"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.a,{target:"_blank","data-noBrokenLinkCheck":!0,href:i(3509).A+"",children:"DS3231 Datasheet"})}),"\n"]}),"\n",(0,s.jsx)(n.h3,{id:"actuator-096-12864-oled-display-module",children:"Actuator: 0.96\u201d 128\xd764 OLED Display Module"}),"\n",(0,s.jsx)(n.p,{children:"The OLED (Organic Light-Emitting Diode) Display, measuring 0.96 inches with a resolution of 128x64 pixels, is a compact and versatile screen commonly used in electronics projects. OLED technology enables each pixel to emit its light, resulting in vibrant and high-contrast displays with deep black levels. This particular display offers excellent readability, even in low-light conditions."}),"\n",(0,s.jsx)("img",{src:h,alt:"0.96\u201d 128\xd764 OLED Display Module",style:{width:"75%",display:"block",margin:"20px auto"}}),"\n",(0,s.jsx)(n.h4,{id:"technical-specifications-and-features-4",children:"Technical Specifications and Features:"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"OLED Driver IC"}),": SSD1306"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Resolution"}),": 128 x 64 pixels"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Visual Angle"}),": >160\xb0"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Display Color"}),": Area Color (White)"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Input Voltage"}),": 3.3V - 6V"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Compatible I/O Level"}),": 3.3V, 5V"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Control Interface"}),": Only requires 2 I/O ports for control"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Working Temperature"}),": -30\xb0C to 70\xb0C"]}),"\n",(0,s.jsxs)(n.li,{children:[(0,s.jsx)(n.strong,{children:"Interface"}),": I2C"]}),"\n"]}),"\n",(0,s.jsx)(n.h4,{id:"references-4",children:"References:"}),"\n",(0,s.jsxs)(n.ul,{children:["\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.a,{target:"_blank","data-noBrokenLinkCheck":!0,href:i(6130).A+"",children:"0.96\u201d 128\xd734 OLED Display Module Datasheet"})}),"\n",(0,s.jsx)(n.li,{children:(0,s.jsx)(n.a,{target:"_blank","data-noBrokenLinkCheck":!0,href:i(5562).A+"",children:"OLED SSD1306 Advance Information"})}),"\n"]}),"\n",(0,s.jsx)(n.h2,{id:"communication-protocols",children:"Communication Protocols"}),"\n",(0,s.jsx)(n.h3,{id:"controller-communication",children:"Controller Communication"}),"\n",(0,s.jsx)(n.p,{children:"The Arduino Nano 33 IoT controller communicates with the ThingsBoard server using two key protocols: Wi-Fi and MQTT."}),"\n",(0,s.jsx)(n.h4,{id:"wi-fi-connection",children:"Wi-Fi Connection"}),"\n",(0,s.jsx)(n.p,{children:"The Arduino Nano 33 IoT connects to a local Wi-Fi network, granting it access to the internet. This connection serves as the foundation for communication with external services like the ThingsBoard server."}),"\n",(0,s.jsx)(n.h4,{id:"mqtt-protocol",children:"MQTT Protocol"}),"\n",(0,s.jsx)(n.p,{children:"For transmitting data between the controller and the ThingsBoard server, the Message Queuing Telemetry Transport (MQTT) protocol is utilized. MQTT is chosen for its lightweight and efficient design, making it suitable for remote devices with limited resources. It enables the publication of data from the controller to the server and supports subscriptions to commands or acknowledgments from the server. MQTT ensures secure and reliable communication, essential for real-time data visualization and monitoring on the ThingsBoard dashboard. Access events generated by the controller are published to the server and visualized on the dashboard, providing insights into system performance and access patterns."}),"\n",(0,s.jsx)(n.h3,{id:"sensor-communication",children:"Sensor Communication"}),"\n",(0,s.jsx)(n.p,{children:"The communication between the Arduino Nano 33 IoT controller and various sensors employed in the node\u2014BMP280, AM2301, and DS3231\u2014utilizes the Inter-Integrated Circuit (I2C) protocol."}),"\n",(0,s.jsx)(n.h4,{id:"i2c-protocol-for-sensors",children:"I2C Protocol (for Sensors)"}),"\n",(0,s.jsx)(n.p,{children:"The I2C protocol facilitates communication between the controller and connected sensors. The BMP280 for pressure and altitude, AM2301 for temperature, relative humidity, and heat index, and DS3231 for real-time clock information all employ the I2C protocol. I2C allows the controller to read data from these sensors, ensuring synchronized and accurate measurements. Its efficiency is crucial for coordinating data collection from multiple sensors simultaneously, providing precise weather information."}),"\n",(0,s.jsx)(n.h3,{id:"actuator-communication",children:"Actuator Communication"}),"\n",(0,s.jsx)(n.p,{children:'The 0.96" 128x64 OLED display, serving as both an actuator and visual output for presenting weather data, communicates with the Arduino Nano 33 IoT controller through the I2C protocol.'}),"\n",(0,s.jsx)(n.h4,{id:"i2c-protocol-for-actuator---oled-display",children:"I2C Protocol (for Actuator - OLED Display)"}),"\n",(0,s.jsx)(n.p,{children:"The OLED display communicates with the controller via the I2C protocol, enabling the controller to send data for presenting weather-related information visually. The I2C protocol's efficiency ensures seamless data exchange between the controller and the OLED display, facilitating real-time weather updates for users."}),"\n",(0,s.jsx)(n.p,{children:"The integration of these communication protocols ensures efficient data collection, processing, and presentation of weather data while maintaining secure and reliable connectivity with the ThingsBoard platform for real-time data visualization and monitoring."}),"\n",(0,s.jsx)(n.h2,{id:"pin-connections",children:"Pin Connections"}),"\n",(0,s.jsx)(n.p,{children:"This section provides details about the pin connections between the components used in the node and the Arduino Nano 33 IoT controller."}),"\n",(0,s.jsx)(n.h3,{id:"bmp280-module-pin-connections",children:"BMP280 Module Pin Connections"}),"\n",(0,s.jsx)(u.default,{data:[["BMP280","","Arduino Nano 33 IoT"],["VCC",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-red"})}),"3.3V"],["GND",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-black"})}),"GND"],["SCL",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-purple"})}),"SCL"],["SDA",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-orange"})}),"SDA"]]}),"\n",(0,s.jsx)(n.h3,{id:"am2301-module-pin-connections",children:"AM2301 Module Pin Connections"}),"\n",(0,s.jsx)(u.default,{data:[["AM2301","","Arduino Nano 33 IoT"],["VCC",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-red"})}),"3.3V"],["GND",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-black"})}),"GND"],["SDA",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-blue"})}),"2"]]}),"\n",(0,s.jsx)(n.h3,{id:"ds3231-module-pin-connections",children:"DS3231 Module Pin Connections"}),"\n",(0,s.jsx)(u.default,{data:[["DS3231","","Arduino Nano 33 IoT"],["VCC",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-red"})}),"3.3V"],["GND",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-black"})}),"GND"],["SCL",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-purple"})}),"SCL"],["SDA",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-orange"})}),"SDA"]]}),"\n",(0,s.jsx)(n.h3,{id:"096-12864-oled-module-pin-connections",children:"0.96\u201d 128\xd764 OLED Module Pin Connections"}),"\n",(0,s.jsx)(u.default,{data:[["0.96\u201d 128\xd764 OLED","","Arduino Nano 33 IoT"],["VCC",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-red"})}),"3.3V"],["GND",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-black"})}),"GND"],["SCL",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-purple"})}),"SCL"],["SDA",(0,s.jsx)(n.span,{className:"wire",children:(0,s.jsx)(n.span,{className:"wire-orange"})}),"SDA"]]}),"\n",(0,s.jsx)(n.h2,{id:"circuit-diagram",children:"Circuit Diagram"}),"\n",(0,s.jsx)("img",{src:p,alt:"Circuit Diagram",style:{display:"block",margin:"20px auto"}}),"\n",(0,s.jsx)(n.h2,{id:"link-to-code-explanation-and-tutorial",children:"Link to Code Explanation and Tutorial"}),"\n",(0,s.jsx)(n.p,{children:"For a comprehensive guide on programming this node, including detailed code explanations and step-by-step tutorials, please refer to the following link. This resource provides all necessary information to understand the functioning of the node, enabling users to effectively program and customize it according to their specific requirements."}),"\n",(0,s.jsx)(n.p,{children:(0,s.jsx)(n.a,{href:"/ACPS-Lab/docs/tutorial/node_code_explained",children:"Visit the Node Programming Guide and Code Explanation"})})]})}function y(e={}){const{wrapper:n}={...(0,r.R)(),...e.components};return n?(0,s.jsx)(n,{...e,children:(0,s.jsx)(v,{...e})}):v(e)}},380:(e,n,i)=>{i.r(n),i.d(n,{default:()=>r});i(6540);var s=i(4848);const r=e=>{let{title:n,data:i}=e;return Array.isArray(i)?(0,s.jsx)("table",{className:"tabler connections",children:(0,s.jsx)("tbody",{children:i.map(((e,n)=>(0,s.jsx)("tr",{className:0===n?"tabler-header":"",children:e.map(((e,n)=>(0,s.jsx)("td",{children:e},n)))},n)))})}):(0,s.jsx)("div",{children:"Loading data..."})}},8559:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/files/AM2301-5e9c0d1c4f822ea76462345a8fdd53cd.pdf"},3624:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/files/Arduino_Nano_33_IoT-4e1a0f88dada4b7447e81712d4939b60.pdf"},72:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/files/Arduino_Nano_33_IoT_Pinout-4cbfbb72283976469dc09308e7f19d58.pdf"},2796:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/files/BMP280-2ad3f6599aa0fc967076524fa14d1ed8.pdf"},3509:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/files/DS3231-4f97eb4238143974d387bf1368ae7286.pdf"},6130:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/files/OLED_128_64-3ff45b6c8c7c3f66a5da9d94ac802e4d.pdf"},5562:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/files/OLED_SSD1306-21d4b7030fa5d4af3d8f1ca37fdeca58.pdf"},237:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/images/Arduino_Nano_33_IoT-c0c8174db8935a13cc24fd7ed9e9c369.png"},5358:(e,n,i)=>{i.d(n,{A:()=>s});const s=i.p+"assets/images/Arduino_Nano_33_IoT_2-45034393adf410ac1d29943b6e4c8925.png"},8453:(e,n,i)=>{i.d(n,{R:()=>a,x:()=>o});var s=i(6540);const r={},t=s.createContext(r);function a(e){const n=s.useContext(t);return s.useMemo((function(){return"function"==typeof e?e(n):{...n,...e}}),[n,e])}function o(e){let n;return n=e.disableParentContext?"function"==typeof e.components?e.components(r):e.components||r:a(e.components),s.createElement(t.Provider,{value:n},e.children)}}}]);