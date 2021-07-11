// HTTP server for the led bar webapp

const express = require("express");

const PORT = process.env.PORT || 3001;

const app = express();

app.get('/', function (req, res) {
  // res.send('GET request to homepage');
  res.json({ message: "Hello from server!" });
});

app.listen(PORT, () => {
  console.log(`Server listening on ${PORT}`);
});
