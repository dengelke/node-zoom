'use strict';

var zoom = require('bindings')('zoom');
var Connection = require('./connection');
var Query = require('./query');
var ResultSet = require('./resultset');

module.exports = {
  Connection: Connection,
  Query: Query,
  Record: zoom.record,
  ResultSet: ResultSet
};
