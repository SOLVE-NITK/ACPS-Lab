import React from 'react';
import './wires.css'; // Import CSS file

const PinConnectionsTable = ({ title, data }) => {
  if (!Array.isArray(data)) {
    return <div>Loading data...</div>; // Or any other placeholder content
  }
  return (
    <table className="tabler connections">
      <tbody>
        {data.map((row, index) => (
          <tr key={index} className={index === 0 ? "tabler-header" : ""}>
            {row.map((cell, cellIndex) => (
              <td key={cellIndex}>{cell}</td>
            ))}
          </tr>
        ))}
      </tbody>
    </table>
  );
};

export default PinConnectionsTable;
