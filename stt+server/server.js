var express = require("express");
var cors = require('cors')
var bodyParser = require('body-parser');



const app = express();
const port = 3002;
var jsonParser = bodyParser.json()
let message = "Hello from server";


app.use(cors());
app.get('/', (req, res) => {
  res.send(message);
});
app.post('/', jsonParser, (req, res) => {
    console.log('receiving data ...');
    console.log('body is ', req.body.new);
    
    message = req.body.new;
    res.send(message);
  });
app.listen(port, () => {
  console.log(`Server is running on port ${port}`);
});