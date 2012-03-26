static string JS_server_server = 
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief JavaScript server functions\n"
  "///\n"
  "/// @file\n"
  "///\n"
  "/// DISCLAIMER\n"
  "///\n"
  "/// Copyright 2011-2012 triagens GmbH, Cologne, Germany\n"
  "///\n"
  "/// Licensed under the Apache License, Version 2.0 (the \"License\");\n"
  "/// you may not use this file except in compliance with the License.\n"
  "/// You may obtain a copy of the License at\n"
  "///\n"
  "///     http://www.apache.org/licenses/LICENSE-2.0\n"
  "///\n"
  "/// Unless required by applicable law or agreed to in writing, software\n"
  "/// distributed under the License is distributed on an \"AS IS\" BASIS,\n"
  "/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
  "/// See the License for the specific language governing permissions and\n"
  "/// limitations under the License.\n"
  "///\n"
  "/// Copyright holder is triAGENS GmbH, Cologne, Germany\n"
  "///\n"
  "/// @author Dr. Frank Celler\n"
  "/// @author Copyright 2011-2012, triAGENS GmbH, Cologne, Germany\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "// -----------------------------------------------------------------------------\n"
  "// --SECTION--                                                 Module \"internal\"\n"
  "// -----------------------------------------------------------------------------\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @addtogroup V8ModuleInternal\n"
  "/// @{\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief internal module\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "ModuleCache[\"/internal\"].exports.db = db;\n"
  "ModuleCache[\"/internal\"].exports.edges = edges;\n"
  "ModuleCache[\"/internal\"].exports.AvocadoCollection = AvocadoCollection;\n"
  "ModuleCache[\"/internal\"].exports.AvocadoEdgesCollection = AvocadoEdgesCollection;\n"
  "\n"
  "if (typeof SYS_DEFINE_ACTION === \"undefined\") {\n"
  "  ModuleCache[\"/internal\"].exports.defineAction = function() {\n"
  "    console.error(\"SYS_DEFINE_ACTION not available\");\n"
  "  }\n"
  "}\n"
  "else {\n"
  "  ModuleCache[\"/internal\"].exports.defineAction = SYS_DEFINE_ACTION;\n"
  "}\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @}\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "// -----------------------------------------------------------------------------\n"
  "// --SECTION--                                             Module \"simple-query\"\n"
  "// -----------------------------------------------------------------------------\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @addtogroup V8ModuleSimpleQuery\n"
  "/// @{\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "try {\n"
  "  require(\"simple-query\");\n"
  "}\n"
  "catch (err) {\n"
  "  console.error(\"while loading 'simple-query' module: %s\", err);\n"
  "}\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @}\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "// -----------------------------------------------------------------------------\n"
  "// --SECTION--                                                        ShapedJson\n"
  "// -----------------------------------------------------------------------------\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @addtogroup V8Shell\n"
  "/// @{\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief prints a shaped json\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "ShapedJson.prototype._PRINT = function(seen, path, names, level) {\n"
  "  if (this instanceof ShapedJson) {\n"
  "    PRINT_OBJECT(this, seen, path, names, level);\n"
  "  }\n"
  "  else {\n"
  "    internal.output(this.toString());\n"
  "  }\n"
  "}\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @}\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "// -----------------------------------------------------------------------------\n"
  "// --SECTION--                                                   AvocadoDatabase\n"
  "// -----------------------------------------------------------------------------\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @addtogroup V8Shell\n"
  "/// @{\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief drops a collection\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "AvocadoDatabase.prototype._drop = function(name) {\n"
  "  var collection = name;\n"
  "\n"
  "  if (typeof name === \"string\") {\n"
  "    collection = db[name];\n"
  "  }\n"
  "\n"
  "  // new born collection\n"
  "  if (collection.status() == 1) {\n"
  "    return;\n"
  "  }\n"
  "\n"
  "  // drop all indexes\n"
  "  var idx = collection.getIndexes();\n"
  "\n"
  "  for (var i = 0;  i < idx.length;  ++i) {\n"
  "    collection.dropIndex(idx[i].iid);\n"
  "  }\n"
  "\n"
  "  // delete all documents\n"
  "  var all = collection.all();\n"
  "\n"
  "  while (all.hasNext()) {\n"
  "    var ref = all.nextRef();\n"
  "\n"
  "    collection.delete(ref);\n"
  "  }\n"
  "}\n"
  "\n"
  "AvocadoEdges.prototype._drop = AvocadoDatabase._drop;\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @}\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "// -----------------------------------------------------------------------------\n"
  "// --SECTION--                                                 AvocadoCollection\n"
  "// -----------------------------------------------------------------------------\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @addtogroup V8Shell\n"
  "/// @{\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief collection is new born\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "AvocadoCollection.STATUS_NEW_BORN = 1;\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief collection is unloaded\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "AvocadoCollection.STATUS_UNLOADED = 2;\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief collection is loaded\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "AvocadoCollection.STATUS_LOADED = 3;\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief collection is corrupted\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "AvocadoCollection.STATUS_CORRUPTED = 4;\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief collection is deleted\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "AvocadoCollection.STATUS_DELETED = 5;\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief collection is unloading\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "AvocadoCollection.STATUS_UNLOADING = 6;\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief drops a collection\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "AvocadoCollection.prototype.drop = function() {\n"
  "  db._drop(this);\n"
  "}\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "}\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief prints a collection\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "AvocadoCollection.prototype._PRINT = function() {\n"
  "  var status = \"unknown\";\n"
  "\n"
  "  switch (this.status()) {\n"
  "    case AvocadoCollection.STATUS_NEW_BORN: status = \"new born\"; break;\n"
  "    case AvocadoCollection.STATUS_UNLOADED: status = \"unloaded\"; break;\n"
  "    case AvocadoCollection.STATUS_UNLOADING: status = \"unloading\"; break;\n"
  "    case AvocadoCollection.STATUS_LOADED: status = \"loaded\"; break;\n"
  "    case AvocadoCollection.STATUS_CORRUPTED: status = \"corrupted\"; break;\n"
  "    case AvocadoCollection.STATUS_DELETED: status = \"deleted\"; break;\n"
  "  }\n"
  "  \n"
  "  SYS_OUTPUT(\"[AvocadoCollection \", this._id, \", \\\"\", this.name(), \"\\\" (status \", status, \")]\");\n"
  "/// @}\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "// Local Variables:\n"
  "// mode: outline-minor\n"
  "// outline-regexp: \"^\\\\(/// @brief\\\\|/// @addtogroup\\\\|// --SECTION--\\\\|/// @page\\\\|/// @}\\\\)\"\n"
  "// End:\n"
;
