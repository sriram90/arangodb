////////////////////////////////////////////////////////////////////////////////
/// @brief rest server options
///
/// @file
/// This file contains the description of the rest server.
///
/// DISCLAIMER
///
/// Copyright 2010-2011 triagens GmbH, Cologne, Germany
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
/// @author Dr. Frank Celler
/// @author Copyright 2010-2011, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef TRIAGENS_REST_REST_MODEL_LOADER_H
#define TRIAGENS_REST_REST_MODEL_LOADER_H 1

#include <Basics/Common.h>

namespace triagens {
  namespace rest {

    ////////////////////////////////////////////////////////////////////////////////
    /// @ingroup RestServer
    /// @brief rest model loader
    ////////////////////////////////////////////////////////////////////////////////

    class RestModelLoader : noncopyable {
      public:
        RestModel* model;

      public:
        RestModelLoader () : model(0) {
        }

      public:
        bool processHandle (string const& /* name */, void* init, void* handle) {
          typedef RestModel* (*func_ptr) ();

          union {
            void* from;
            func_ptr to;
          } cnv;

          cnv.from = init;
          model = cnv.to();

          return true;
        }
    };
  }
}

#endif


