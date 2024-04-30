import type {SidebarsConfig} from '@docusaurus/plugin-content-docs';

/**
 * Creating a sidebar enables you to:
 - create an ordered group of docs
 - render a sidebar for each doc of that group
 - provide next/previous navigation

 The sidebars can be generated from the filesystem, or explicitly defined here.

 Create as many sidebars as you want.
 */
const sidebars: SidebarsConfig = {
  // By default, Docusaurus generates a sidebar from the docs folder structure
  docs: [
    'intro',
    'system_architecture',
    {
      type: 'category',
      label: 'Physical Components',
      link: {
        type: 'generated-index',
      },
      collapsed: true,
      items: [
        'physical_components/general_node_architecture',
        'physical_components/node1',
        'physical_components/node2',
        'physical_components/node3',
        'physical_components/node4',
        'physical_components/node5',
        'physical_components/node6',
        'physical_components/node7',
        'physical_components/node8',
        'physical_components/node9',
        'physical_components/node10',
        'physical_components/node11',
        'physical_components/node12',
      ],
    },
    {
      type: 'category',
      label: 'Cyber Components',
      link: {
        type: 'generated-index',
      },
      items: [
        'cyber_components/outline',
        'cyber_components/thingsboard_platform',
        'cyber_components/thingsboard_data_visualization',
        'cyber_components/device_connectivity_application_dev',
        'cyber_components/rpi',
      ],
    },
    {
      type: 'category',
      label: 'Communication Components',
      link: {
        type: 'generated-index',
      },
      items: [
        'communication_components/overview',
        'communication_components/wired_communication_protocols',
        'communication_components/wireless_communication_protocols',
        'communication_components/software_communication_protocols',
      ],
    },
  ],
      
  tutorialSidebar: [
    'tutorial/installation',
    'tutorial/node_code_explained',
    'tutorial/thingsboard',
    'tutorial/experimental_setups',
  ],
};

export default sidebars;
