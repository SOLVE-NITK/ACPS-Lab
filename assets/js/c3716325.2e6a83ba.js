/*! For license information please see c3716325.2e6a83ba.js.LICENSE.txt */
"use strict";(self.webpackChunkacps_documentation=self.webpackChunkacps_documentation||[]).push([[955],{5352:(e,t,i)=>{i.r(t),i.d(t,{assets:()=>l,contentTitle:()=>r,default:()=>m,frontMatter:()=>s,metadata:()=>c,toc:()=>d});var n=i(4848),o=i(8453),a=i(3852);const s={title:"Software Communication Protocols",description:"Covers the implementation of software protocols like MQTT for messaging, HTTP for data fetching, and UDP for real-time data transmission, emphasizing their impact on system functionality."},r="Software Communication Protocols",c={id:"communication_components/software_communication_protocols",title:"Software Communication Protocols",description:"Covers the implementation of software protocols like MQTT for messaging, HTTP for data fetching, and UDP for real-time data transmission, emphasizing their impact on system functionality.",source:"@site/docs/communication_components/software_communication_protocols.mdx",sourceDirName:"communication_components",slug:"/communication_components/software_communication_protocols",permalink:"/docs/communication_components/software_communication_protocols",draft:!1,unlisted:!1,tags:[],version:"current",frontMatter:{title:"Software Communication Protocols",description:"Covers the implementation of software protocols like MQTT for messaging, HTTP for data fetching, and UDP for real-time data transmission, emphasizing their impact on system functionality."},sidebar:"docs",previous:{title:"Wireless Communication Protocols",permalink:"/docs/communication_components/wireless_communication_protocols"}},l={},d=[{value:"MQTT (Message Queuing Telemetry Transport)",id:"mqtt-message-queuing-telemetry-transport",level:2},{value:"Basics",id:"basics",level:4},{value:"Working",id:"working",level:4},{value:"HTTP (Hypertext Transfer Protocol)",id:"http-hypertext-transfer-protocol",level:2},{value:"Basics",id:"basics-1",level:4},{value:"Working",id:"working-1",level:4},{value:"UDP (User Datagram Protocol)",id:"udp-user-datagram-protocol",level:2},{value:"Basics",id:"basics-2",level:4},{value:"Working",id:"working-2",level:4},{value:"Future Scope of Integrating Other Protocols",id:"future-scope-of-integrating-other-protocols",level:2},{value:"AMQP (Advanced Message Queuing Protocol)",id:"amqp-advanced-message-queuing-protocol",level:3},{value:"CoAP (Constrained Application Protocol)",id:"coap-constrained-application-protocol",level:3},{value:"LwM2M (Lightweight Machine to Machine)",id:"lwm2m-lightweight-machine-to-machine",level:3},{value:"DDS (Data Distribution Service)",id:"dds-data-distribution-service",level:3}];function p(e){const t={h1:"h1",h2:"h2",h3:"h3",h4:"h4",li:"li",p:"p",strong:"strong",ul:"ul",...(0,o.R)(),...e.components};return(0,n.jsxs)(n.Fragment,{children:[(0,n.jsx)(t.h1,{id:"software-communication-protocols",children:"Software Communication Protocols"}),"\n",(0,n.jsx)(t.p,{children:"Software communication protocols are essential for efficient data transfer and management in Applied Cyber-Physical Systems (ACPS). This section explores the primary protocols currently in use\u2014MQTT, HTTP, and UDP\u2014and considers future integration possibilities to enhance system capabilities."}),"\n",(0,n.jsx)(t.h2,{id:"mqtt-message-queuing-telemetry-transport",children:"MQTT (Message Queuing Telemetry Transport)"}),"\n",(0,n.jsx)(t.h4,{id:"basics",children:"Basics"}),"\n",(0,n.jsx)(t.p,{children:"MQTT is a lightweight messaging protocol designed for low-bandwidth, high-latency environments. It is ideal for IoT applications where bandwidth efficiency and low power consumption are crucial."}),"\n",(0,n.jsx)(t.h4,{id:"working",children:"Working"}),"\n",(0,n.jsxs)(t.ul,{children:["\n",(0,n.jsxs)(t.li,{children:[(0,n.jsx)(t.strong,{children:"Publish/Subscribe Model"}),": MQTT operates on a publish/subscribe model where messages are published by devices to topics, which are subscribed to by clients. The broker facilitates this exchange, ensuring messages reach the intended subscribers."]}),"\n",(0,n.jsxs)(t.li,{children:[(0,n.jsx)(t.strong,{children:"Reliability and Efficiency"}),": Provides multiple levels of Quality of Service (QoS) to balance between latency, bandwidth, and message reliability."]}),"\n"]}),"\n",(0,n.jsxs)("div",{style:{position:"relative",width:"100%",display:"inline-block"},children:[(0,n.jsx)("div",{className:"video-container",children:(0,n.jsx)(a.A,{id:"EIxdz-2rhLs",title:"What is MQTT and How It Works"})}),(0,n.jsx)("div",{style:{fontSize:"14px",position:"absolute",bottom:-55,right:0,padding:"5px"},children:(0,n.jsxs)(t.p,{children:["Source: ",(0,n.jsx)("a",{href:"https://www.youtube.com/@RuiSantosdotme",target:"_blank",children:"Rui Santos YouTube Channel"})]})})]}),"\n",(0,n.jsx)(t.h2,{id:"http-hypertext-transfer-protocol",children:"HTTP (Hypertext Transfer Protocol)"}),"\n",(0,n.jsx)(t.h4,{id:"basics-1",children:"Basics"}),"\n",(0,n.jsx)(t.p,{children:"HTTP is a foundational web protocol used for data communication on the World Wide Web, facilitating the transmission of multimedia and hypermedia across the internet."}),"\n",(0,n.jsx)(t.h4,{id:"working-1",children:"Working"}),"\n",(0,n.jsxs)(t.ul,{children:["\n",(0,n.jsxs)(t.li,{children:[(0,n.jsx)(t.strong,{children:"Request/Response Model"}),": HTTP operates on a request/response model where clients send requests to servers to retrieve web pages or resources, and servers respond with the requested content."]}),"\n",(0,n.jsxs)(t.li,{children:[(0,n.jsx)(t.strong,{children:"Stateless Protocol"}),": Each request is independent; servers do not retain session information, which simplifies network scaling."]}),"\n"]}),"\n",(0,n.jsxs)("div",{style:{position:"relative",width:"100%",display:"inline-block"},children:[(0,n.jsx)("div",{className:"video-container",children:(0,n.jsx)(a.A,{id:"-mN3VyJuCjM",title:"What Is REST API? Examples And How To Use It"})}),(0,n.jsx)("div",{style:{fontSize:"14px",position:"absolute",bottom:-55,right:0,padding:"5px"},children:(0,n.jsxs)(t.p,{children:["Source: ",(0,n.jsx)("a",{href:"https://www.youtube.com/@ByteByteGo",target:"_blank",children:"ByteByteGo YouTube Channel"})]})})]}),"\n",(0,n.jsx)(t.h2,{id:"udp-user-datagram-protocol",children:"UDP (User Datagram Protocol)"}),"\n",(0,n.jsx)(t.h4,{id:"basics-2",children:"Basics"}),"\n",(0,n.jsx)(t.p,{children:"UDP is a connectionless protocol that allows the transmission of datagrams without assurances for delivery, order, or duplicate protection, making it faster and suitable for applications requiring low latency."}),"\n",(0,n.jsx)(t.h4,{id:"working-2",children:"Working"}),"\n",(0,n.jsxs)(t.ul,{children:["\n",(0,n.jsxs)(t.li,{children:[(0,n.jsx)(t.strong,{children:"Speed and Simplicity"}),": UDP does not establish a connection before sending data and does not guarantee error checking or recovery, hence it is faster and more suitable for real-time applications such as video streaming or gaming."]}),"\n"]}),"\n",(0,n.jsxs)("div",{style:{position:"relative",width:"100%",display:"inline-block"},children:[(0,n.jsx)("div",{className:"video-container",children:(0,n.jsx)(a.A,{id:"HMKC3RSUuJg",title:"User Datagram Protocol Explained "})}),(0,n.jsx)("div",{style:{fontSize:"14px",position:"absolute",bottom:-55,right:0,padding:"5px"},children:(0,n.jsxs)(t.p,{children:["Source: ",(0,n.jsx)("a",{href:"https://www.youtube.com/@SimplilearnOfficial",target:"_blank",children:"Simplilearn YouTube Channel"})]})})]}),"\n",(0,n.jsx)(t.h2,{id:"future-scope-of-integrating-other-protocols",children:"Future Scope of Integrating Other Protocols"}),"\n",(0,n.jsx)(t.p,{children:"The integration of additional, more specialized protocols can further enhance the communication flexibility and efficiency in ACPS."}),"\n",(0,n.jsx)(t.h3,{id:"amqp-advanced-message-queuing-protocol",children:"AMQP (Advanced Message Queuing Protocol)"}),"\n",(0,n.jsx)(t.p,{children:"Provides robust messaging capabilities with features like message orientation, queuing, routing, reliability, and security, ideal for enterprise-level applications requiring complex transaction management."}),"\n",(0,n.jsx)(t.h3,{id:"coap-constrained-application-protocol",children:"CoAP (Constrained Application Protocol)"}),"\n",(0,n.jsx)(t.p,{children:"Designed specifically for simple electronic devices, CoAP operates over UDP and is highly efficient for constrained environments, making it suitable for M2M (machine-to-machine) applications in IoT ecosystems."}),"\n",(0,n.jsx)(t.h3,{id:"lwm2m-lightweight-machine-to-machine",children:"LwM2M (Lightweight Machine to Machine)"}),"\n",(0,n.jsx)(t.p,{children:"A protocol from the Open Mobile Alliance for device management and telemetry in IoT that uses CoAP as a transport layer, providing a lightweight and secure way to manage IoT devices."}),"\n",(0,n.jsx)(t.h3,{id:"dds-data-distribution-service",children:"DDS (Data Distribution Service)"}),"\n",(0,n.jsx)(t.p,{children:"A middleware protocol for real-time systems that provides low-latency data distribution, high reliability, and scalable architecture, ideal for complex systems requiring real-time data processing and distribution."})]})}function m(e={}){const{wrapper:t}={...(0,o.R)(),...e.components};return t?(0,n.jsx)(t,{...e,children:(0,n.jsx)(p,{...e})}):p(e)}},3852:(e,t,i)=>{i.d(t,{A:()=>a});var n=i(6540),o=function(){return o=Object.assign||function(e){for(var t,i=1,n=arguments.length;i<n;i++)for(var o in t=arguments[i])Object.prototype.hasOwnProperty.call(t,o)&&(e[o]=t[o]);return e},o.apply(this,arguments)};var a=n.forwardRef((function(e,t){var i=n.useState(!1),a=i[0],s=i[1],r=n.useState(!1),c=r[0],l=r[1],d=encodeURIComponent(e.id),p="string"==typeof e.playlistCoverId?encodeURIComponent(e.playlistCoverId):null,m=e.title,h=e.poster||"hqdefault",u="&".concat(e.params)||0,g=e.muted?"&mute=1":"",f=e.announce||"Watch",v=e.webp?"webp":"jpg",b=e.webp?"vi_webp":"vi",w=e.thumbnail||(e.playlist?"https://i.ytimg.com/".concat(b,"/").concat(p,"/").concat(h,".").concat(v):"https://i.ytimg.com/".concat(b,"/").concat(d,"/").concat(h,".").concat(v)),y=e.noCookie;y=e.cookie?"https://www.youtube.com":"https://www.youtube-nocookie.com";var x=e.playlist?"".concat(y,"/embed/videoseries?autoplay=1").concat(g,"&list=").concat(d).concat(u):"".concat(y,"/embed/").concat(d,"?autoplay=1&state=1").concat(g).concat(u),j=e.activatedClass||"lyt-activated",T=e.adNetwork||!1,P=e.aspectHeight||9,k=e.aspectWidth||16,C=e.iframeClass||"",S=e.playerClass||"lty-playbtn",M=e.wrapperClass||"yt-lite",A=e.onIframeAdded||function(){},D=e.rel?"prefetch":"preload",_=e.containerElement||"article";return n.useEffect((function(){c&&A()}),[c]),n.createElement(n.Fragment,null,n.createElement("link",{rel:D,href:w,as:"image"}),n.createElement(n.Fragment,null,a&&n.createElement(n.Fragment,null,n.createElement("link",{rel:"preconnect",href:y}),n.createElement("link",{rel:"preconnect",href:"https://www.google.com"}),T&&n.createElement(n.Fragment,null,n.createElement("link",{rel:"preconnect",href:"https://static.doubleclick.net"}),n.createElement("link",{rel:"preconnect",href:"https://googleads.g.doubleclick.net"})))),n.createElement(_,{onPointerOver:function(){a||s(!0)},onClick:function(){c||l(!0)},className:"".concat(M," ").concat(c?j:""),"data-title":m,style:o({backgroundImage:"url(".concat(w,")")},{"--aspect-ratio":"".concat(P/k*100,"%")})},n.createElement("button",{type:"button",className:S,"aria-label":"".concat(f," ").concat(m)}),c&&n.createElement("iframe",{ref:t,className:C,title:m,width:"560",height:"315",frameBorder:"0",allow:"accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture",allowFullScreen:!0,src:x})))}))},8453:(e,t,i)=>{i.d(t,{R:()=>s,x:()=>r});var n=i(6540);const o={},a=n.createContext(o);function s(e){const t=n.useContext(a);return n.useMemo((function(){return"function"==typeof e?e(t):{...t,...e}}),[t,e])}function r(e){let t;return t=e.disableParentContext?"function"==typeof e.components?e.components(o):e.components||o:s(e.components),n.createElement(a.Provider,{value:t},e.children)}}}]);