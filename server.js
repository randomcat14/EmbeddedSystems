
import express from "express";
import sgMail from "@sendgrid/mail";
import dotenv from "dotenv";
import path from "path";
import { fileURLToPath } from "url";

dotenv.config();
sgMail.setApiKey(process.env.SENDGRID_API_KEY);

const app = express();
app.use(express.json());

app.post("/api/send-spike-email", async (req, res) => {
  const { email } = req.body;
  if (!email) return res.status(400).json({ ok: false, error: "Missing email" });

  try {
    await sgMail.send({
      to: email,
      from: process.env.MAIL_FROM, 
      subject: "Spike alert",
      html: "<h1>Warning!</h1><p>This is a warning of the following: WEEEEE</p>",
    });
    return res.json({ ok: true });
  } catch (err) {
    console.error("SendGrid error code:", err?.code);
    console.error("SendGrid status:", err?.response?.statusCode);
    console.error("SendGrid body:", err?.response?.body);
    const msg =
      err?.response?.body?.errors?.map(e => e.message).join("; ") ||
      err?.message ||
      "Failed to send email";
    return res.status(500).json({ ok: false, error: msg });
  }
});

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
app.use(express.static(path.join(__dirname, "public")));
app.get("/", (_, res) => res.sendFile(path.join(__dirname, "public", "index.html")));

app.listen(3000, () => console.log("✅ http://localhost:3000"));
