
import { Client } from "pg";

const client = new Client({
  user: "my_esp",
  host: "localhost",
  database: "myespdba",
  password: "pass",
  port: 5432,
});

// Connect once and reuse the client
export async function initDb() {
  await client.connect();
  console.log("ected to PostgreSQL");
}

// psqlConnect.js
export async function getLatestPumpReading() {
  const sql = `
    SELECT  voltage1, voltage2, voltage3
    FROM voltage_sensors

  `;

  const { rows } = await client.query(sql);

 
  const v1 = rows.map(row => Number(row.voltage1));         // y for chart 1
  const v3 = rows.map(row => Number(row.voltage2));         // y for chart 2
  const v2 = rows.map(row => Number(row.voltage3));         // y for chart 3

  return { v1, v2, v3 };
}
