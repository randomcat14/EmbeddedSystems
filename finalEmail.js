// emailRoutes.js
import express from "express";
import sgMail from "@sendgrid/mail";
import dotenv from "dotenv";

dotenv.config(); 
sgMail.setApiKey(process.env.SENDGRID_API_KEY);

const router = express.Router();

router.post("/send-email", async (req, res) => {
  const { email } = req.body;
  if (!email) {
    return res.status(400).json({ ok: false, error: "Missing email" });
  }

  try {
    await sgMail.send({
      to: email,
      from: process.env.MAIL_FROM,
      subject: "HELP! THEY ARE ROASTING MY GUMDROP BUTTONS",
      html: `
        <h1>GINGY IS UNDER ATTACK</h1>
        <p>WEEEEE</p>
        <p>Fire Burning in the house°F.</p>
      `,
    });

    return res.json({ ok: true });
  } catch (err) {
    console.error("SendGrid error code:", err?.code);
    console.error("SendGrid status:", err?.response?.statusCode);
    console.error("SendGrid body:", err?.response?.body);

    const msg =
      err?.response?.body?.errors?.map((e) => e.message).join("; ") ||
      err?.message ||
      "Failed to send email";

    return res.status(500).json({ ok: false, error: msg });
  }
});

export default router;
