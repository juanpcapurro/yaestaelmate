#! /usr/bin/env node
var jayson  = require('jayson');
const child_process = require('child_process');
var cors = require('cors');
var connect = require('connect');
var jsonParser = require('body-parser').json;
var app = connect();

var server = jayson.server({
  ya_esta_el_mate: function(args, callback) {
    if (typeof args[0] !== 'string'){
      child_process.spawn('notify-send', ['ya esta el mate']);
    }else{
      child_process.spawn('notify-send', [args[0]]);
    }
    callback(null,);
  }
});

app.use(cors({methods: ['POST']}));
app.use(jsonParser());
app.use(server.middleware());

app.listen(3000);
