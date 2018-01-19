#! /usr/bin/env node
var jayson          = require('jayson');
var cors            = require('cors');
var connect         = require('connect');
var jsonParser      = require('body-parser').json;
var notifier        = require('node-notifier');
var app             = connect();

var server = jayson.server({
  ya_esta_el_mate: function(args, callback) {
    notifier.notify({
      title: (typeof args[0] === 'string') ? args[0] + ' ': 'ya esta el mate',
      message:(typeof args[1] === 'string') ? args[1] + ' ' : ' '
    });
    callback(null);
  }
});

app.use(cors({methods: ['POST']}));
app.use(jsonParser());
app.use(server.middleware());

app.listen(3000);
