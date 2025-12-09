
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

export async function getLatestPumpReading() {
  const sql = `
    SELECT * 
    FROM voltage_sensors;
  `;
  return client.query(sql);
}
