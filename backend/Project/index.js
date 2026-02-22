import express from "express";
const app = express();
const port = 3000;

app.get("/", (req, res) => {
    res.send("<h1>Hi</h1>");
});
app.get("/about", (req, res) => {
    res.send("<h1>About Me</h1><p>Myself Aminur</p>");
});
app.get("/contact", (req, res) => {
    res.send("<h1>Contact me at</h1><p>+91123145678</p>");
});

app.listen(3000, () => {
    console.log(`Server running on port ${port}`);
});