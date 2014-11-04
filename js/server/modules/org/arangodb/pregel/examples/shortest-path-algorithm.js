/*jslint indent: 2, nomen: true, maxlen: 120, todo: true, white: false, sloppy: false */
/*global exports*/

////////////////////////////////////////////////////////////////////////////////
/// @brief pregel implementation of the graph shortes path algorithm
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2010-2014 triagens GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is triAGENS GmbH, Cologne, Germany
///
/// @author Florian Bartels
/// @author Copyright 2014, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

var shortestPath = function (vertex, message, global) {
  'use strict';
  var distanceAttrib = global.distance;
  var direction = global.direction || "outbound";
  var next, e, c = 0;
  if (direction !== "outbound") {
    c = 1;
  }

  var CALC_PATHS = "paths";
  var CALC_CENTRALITY = "centralityMeasures";

  var getDistance = function (edge, distanceAttrib) {
    if (distanceAttrib && edge[distanceAttrib]) {
      return edge[distanceAttrib];
    }
    if (distanceAttrib && edge._get(distanceAttrib)) {
      return edge._get(distanceAttrib);
    }
    if (distanceAttrib) {
      return Infinity;
    }
    return 1;
  };

  var mergePaths = function (pathList1, pathList2) {
    var r = [];
    pathList1.forEach(function (p1) {
      pathList2.forEach(function (p2) {
        r.push(p1.concat(p2));
      });
    });
    return r;
  };

  var result = vertex._getResult() || {
    inbound : [],
    outbound : [],
    sP: {
      root : vertex._get("_id")
    }
  }, saveResult = false;
  if (!result.sP[vertex._get("_id")]) {
    result.sP[vertex._get("_id")] = {
      distance: 0
    };
  }
  var noDeactivate = false;
  var i, keys, v, keys2, s, j, t;
  switch (result.finalStep) {
  case CALC_CENTRALITY:
    result.eccentricity = (1 / result.absEccentricity) / global.maxRevEccentricity;
    result.closeness = (result.absCloseness) / global.maxCloseness;
    result.diameter = global.diameter;
    result.radius = global.radius;
    saveResult = true;
    break;

  case CALC_PATHS:
    while (message.hasNext()) {
      next = message.next();
      if (!next.data.finish) {
        if (!result.sP[next.data.t]) {
          continue;
        }
        if (Math.abs(next.data.d - result.sP[next.data.t].distance) > 0.00001) {
          continue;
        }

        if (result.sP[next.data.t].done) {
          message.sendTo(next.data.r, {
            finish: mergePaths(next.data.p, result.sP[next.data.t].paths),
            t: next.data.t
          });
        } else {
          for (i = 0; i < result.outbound.length; ++i) {
            e = result.outbound[i];
            message.sendTo(e.target, {
              r: next.data.r,
              t: next.data.t,
              d: result.sP[next.data.t].distance - result.sP[e.v].distance,
              p: mergePaths(next.data.p, result.sP[e.v].paths || [])
            });
          }
        }
      } else {
        if (!result.sP[next.data.t].paths) {
          result.sP[next.data.t].paths = next.data.finish;
        } else {
          result.sP[next.data.t].paths = result.sP[next.data.t].paths.concat(next.data.finish);
        }
        result.sP[next.data.t].done = true;
        saveResult = true;
      }

    }

    break;

  default:
    if (global.step === 0 && c === 1) {
      while (vertex._outEdges.hasNext()) {
        e = vertex._outEdges.next();
        message.sendTo(e._getTarget(), {
          vertex : vertex._get("_id"),
          distance : getDistance(e, distanceAttrib)
        });
      }
      noDeactivate = true;
    }
    var data = {};
    if (global.step === c) {
      var _from = vertex._get("_id");
      var _to;
      data[_from] = {};
      while (message.hasNext()) {
        next = message.next();
        _to = next.data.vertex;
        data[_from][_to] = {
          distance: next.data.distance,
          target: next.sender,
          v : _to
        };
        result.sP[_to] = data[_from][_to];
      }
      if (direction !== "inbound") {
        while (vertex._outEdges.hasNext()) {
          e = vertex._outEdges.next();
          _to = e._get("_to");
          if (!data[_from][_to] || data[_from][_to].distance > getDistance(e, distanceAttrib)) {
            data[_from][_to] = {
              distance: getDistance(e, distanceAttrib),
              target: e._getTarget(),
              v : _to
            };
            result.sP[_to] = data[_from][_to];
          }
        }
      }
      Object.keys(data[_from]).forEach(function (to) {
        result.outbound.push(data[_from][to]);
      });
      saveResult = true;
      result.outbound.forEach(function (s) {
        message.sendTo(s.target, data);
      });
    } else if (global.step === 1  + c) {
      while (message.hasNext()) {
        next = message.next();
        data = {};
        keys = Object.keys(next.data);
        for (i = 0; i < keys.length; ++i) {
          v = keys[i];
          data[v] = {};
          keys2 = Object.keys(result.sP);
          for (j = 0; j < keys2; ++j) {
            s = keys2[j];
            data[v][s] = {
              distance: next.data[v][vertex._get("_id")].distance + result.sP[s].distance,
              v : s
            };
          }
          message.sendTo(next.sender, data);
        }
        saveResult = true;
        result.inbound.push(next.sender);
      }
    } else if (global.step === 2 + c) {
      while (message.hasNext()) {
        next = message.next();
        keys = Object.keys(next.data[vertex._get("_id")]);
        for (i = 0; i < keys.length; ++i) {
          t = keys[i];
          if (vertex._get("_id") === t) {
            return;
          }
          if (!result.sP[t]) {
            result.sP[t] = {
              distance: Infinity,
              v : t
            };
            saveResult = true;
          }

          if (result.sP[t].distance > next.data[vertex._get("_id")][t].distance) {
            saveResult = true;
            result.sP[t] = next.data[vertex._get("_id")][t];
          }
        }
      }
      result.inbound.forEach(function (i) {
        message.sendTo(i, result.sP);
      });
    } else {
      var send = false;
      while (message.hasNext()) {
        next = message.next();
        keys = Object.keys(next.data);
        for (i = 0; i < keys.length; ++i) {
          t = keys[i];
          next.data[t].distance = next.data[t].distance + result.sP[next.data.root].distance;
          if (vertex._get("_id") === t) {
            return;
          }
          if (!result.sP[t]) {
            result.sP[t] = {
              distance: Infinity,
              v : t
            };
            saveResult = true;
          }
          if (result.sP[t].distance > next.data[t].distance) {
            result.sP[t] = next.data[t];
            saveResult = true;
            send = true;
          }
        }
      }
      if (send) {
        result.inbound.forEach(function (i) {
          message.sendTo(i, result.sP);
        });
      }
    }
  }

  if (saveResult === true) {
    vertex._setResult(result);
  }
  if (!noDeactivate) {
    vertex._deactivate();
  }
};

var finalAlgorithm = function (vertex, message, global) {
  'use strict';
  var result = vertex._getResult();
  var sP = result.sP;
  if (!global.calculate) {
    return;
  }
  if (global.calculate === "paths") {
    var done = true;
    Object.keys(sP).forEach(function (target) {
      if (!sP[target].done) {
        done = false;
      }
    });
    if (done) {
      return;
    }
    result.finalStep = global.calculate;
    result.outbound.forEach(function (e) {
      if (e.distance === result.sP[e.v].distance) {
        result.sP[e.v].paths = [
          [vertex._get("_id") + "_" + e.v]
        ];
        result.sP[e.v].done = true;
      }
    });
    Object.keys(sP).forEach(function (target) {
      if (result.sP[target].done || target === "root") {
        return;
      }
      if (target === vertex._get("_id")) {
        result.sP[target].paths = [];
        result.sP[target].done = true;
        return;
      }
      result.outbound.forEach(function (e) {
        message.sendTo(e.target, {
          r: vertex._getLocationInfo(),
          t: target,
          d: result.sP[target].distance - result.sP[e.v].distance,
          p: [
            [vertex._get("_id") + "_" + e.v]
          ]
        });
      });
    });
  }
  if (global.calculate === "centralityMeasures" && !global.maxRevEccentricity) {
    Object.keys(sP).forEach(function (t) {
      if (!result.absEccentricity) {
        result.absEccentricity = sP[t].distance;
      } else {
        result.absEccentricity = Math.max(result.absEccentricity, sP[t].distance);
      }
      if (!result.absCloseness) {
        result.absCloseness = sP[t].distance === 0 ? 0 : 1 / sP[t].distance;
      } else {
        result.absCloseness = result.absCloseness + sP[t].distance === 0 ? 0 : 1 / sP[t].distance;
      }
    });
    global.data.push({e: result.absEccentricity, c: result.absCloseness});
    result.finalStep = global.calculate;
    message.sendTo(vertex._getLocationInfo(), {});
  }
  vertex._setResult(result);
  vertex._deactivate();
};

var conductorAlgorithm = function (globals, stepInfo) {
  'use strict';
  if (globals.calculate === "centralityMeasures" && stepInfo.final && stepInfo.data.length > 0) {
    var max = 0, tmp, radius = Infinity, diameter = 0, maxCloseness = 0;
    stepInfo.data.forEach(function (r) {
      tmp = r.e === 0 ? 0 : 1 / r.e;
      if (tmp > max) {
        max = tmp;
      }
      if (r.e > diameter) {
        diameter = r.e;
      }
      if (r.e < radius) {
        radius = r.e;
      }
      if (r.c > maxCloseness) {
        maxCloseness = r.c;
      }
    });
    globals.diameter = diameter;
    globals.radius = radius;
    globals.maxCloseness = maxCloseness;
    globals.maxRevEccentricity = max;
  } else {
    return;
  }
};


var getAlgorithm = function () {
  'use strict';
  return {
    base  : shortestPath.toString(),
    final : finalAlgorithm.toString(),
    superstep : conductorAlgorithm.toString()
  };
};

exports.getAlgorithm = getAlgorithm;
