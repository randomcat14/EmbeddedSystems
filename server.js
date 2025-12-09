// server.js
import express from "express";
import dotenv from "dotenv";
import path from "path";
import { fileURLToPath } from "url";

import emailRouter from "./finalEmail.js";
import { initDb, getLatestPumpReading } from "./psqlConnect.js";

dotenv.config();

const app = express();
app.use(express.json());

// Static files setup (__dirname equivalent in ES modules)
const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

app.use(express.static(path.join(__dirname, "public")));
app.get("/", (_, res) =>
  res.sendFile(path.join(__dirname, "public", "index.html"))
);

// Mount email routes under /api
app.use("/api", emailRouter);



const PORT = process.env.PORT || 3000;

initDb()
  .then(() => {
    app.listen(PORT, () =>
      console.log(`✅ Server running at http://localhost:${PORT}`)
    );
  })
  .catch((err) => {
    console.error("Failed to initialize DB, server not started.");
  });
app.get("/api/test-db", async (_, res) => {
  try {
    const result = await getLatestPumpReading();

    console.log(result.rows);

    return res.json({
      ok: true,
      data: result.rows, // array of rows
    });
  } catch (err) {
    console.error("DB test route error:", err);
    res.status(500).json({ ok: false, error: "DB test failed" });
  }
});
