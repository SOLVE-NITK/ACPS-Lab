import React from 'react';
import Layout from '@theme/Layout';
import Link from '@docusaurus/Link';

import styles from './index.module.css';

export default function Overview() {
  return (
    <Layout title="Overview" description="Overview of the Applied Cyber-Physical Systems (ACPS) Project">
      <div 
      style={{
        // textAlign: 'justify',
        paddingLeft: '5%',
        paddingRight: '5%',
        paddingTop: '2.25%',
        paddingBottom: '5%',
        }}>
          
        <h1>Overview</h1>
        <p>Welcome to the documentation for the Applied Cyber-Physical Systems (ACPS) Project developed at the Centre for System Design (CSD), National Institute of Technology Karnataka. This project showcases an innovative integration of cybernetics, computing technologies, and physical processes. The ACPS project is designed to educate and provide hands-on experience with real-world applications of cyber-physical systems (CPS), leveraging open-source technologies and modular system architecture.</p>

        <h2>Project Significance</h2>
        <p>Cyber-Physical Systems (CPS) represent a groundbreaking integration of computation with physical processes. Through embedded computers and networks, these systems monitor and control physical processes with real-time feedback loops that allow computational systems to influence, and be influenced by, physical states. The potential of CPS extends far beyond current applications, promising substantial economic and societal benefits. Key applications include:</p>
        <ul>
          <li><strong>Smart Infrastructure:</strong> Managing utilities and services like electricity and water through smart grids.</li>
          <li><strong>Healthcare:</strong> Enhancing diagnostic and treatment capabilities through real-time health monitoring systems.</li>
          <li><strong>Transportation:</strong> Improving safety and operational efficiency through autonomous vehicles and intelligent traffic systems.</li>
          <li><strong>Manufacturing:</strong> Boosting productivity and safety with advanced automation and real-time control systems.</li>
          <li><strong>Environmental Monitoring:</strong> Supporting conservation efforts and disaster management through responsive environmental systems.</li>
        </ul>

        <h2>Objectives</h2>
        <p>The ACPS project is committed to advancing the field of Cyber-Physical Systems through the following key objectives:</p>
        <ul>
          <li><strong>Demonstrate the Integral Concepts of CPS:</strong> Provide a tangible platform to demonstrate the integral concepts of CPS, offering learners an experiential understanding that bridges the gap between theory and application.</li>
          <li><strong>Foster a Hands-On Learning Environment:</strong> Establish an immersive laboratory environment where students can interact with both physical and computational elements of CPS in real time.</li>
          <li><strong>Promote Interdisciplinary Collaboration:</strong> Create opportunities for interdisciplinary collaboration among students from various engineering and technology disciplines.</li>
        </ul>

        <h2>Documentation Structure</h2>
        <p>This documentation is organized into several key sections:</p>
        <ul>
          <li><strong>Introduction:</strong> An overview of the project and its broader impact.</li>
          <li><strong>System Architecture:</strong> Detailed exploration of the system's design and how various components interact.</li>
          <li><strong>Physical and Cyber Components:</strong> In-depth descriptions of the hardware and software that power the project.</li>
          <li><strong>Communication Components:</strong> Insights into the communication strategies and protocols used.</li>
          <li><strong>Tutorials:</strong> Step-by-step guides and instructions for installation, node code explanations, and practical use of ThingsBoard, along with detailed experimental setups and demonstrations.</li>
          <li><strong>About:</strong> Information about the team behind the project and acknowledgements to contributors and supporters.</li>
        </ul>

        <h2>Getting Started</h2>
        <p>This documentation will help you navigate through the complexities of setting up and using the Applied Cyber-Physical Systems (ACPS). Whether you are a student, educator, or industry professional, these resources are designed to enhance your understanding and operational capabilities within the realm of cyber-physical systems.</p>
        <p>To begin exploring our detailed documentation, please click the link below:</p>
        <div className={styles.buttons}>
          <Link
            className="button button--secondary button--lg"
            to="/docs/introduction">
            Documentation
          </Link>
        </div>
        </div>
    </Layout>
  );
}