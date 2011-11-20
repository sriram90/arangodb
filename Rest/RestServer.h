////////////////////////////////////////////////////////////////////////////////
/// @brief rest server template
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

#ifndef TRIAGENS_REST_REST_SERVER_H
#define TRIAGENS_REST_REST_SERVER_H 1

#include <Rest/AnyServer.h>

////////////////////////////////////////////////////////////////////////////////
/// @defgroup RestServer Rest-Server and Model
////////////////////////////////////////////////////////////////////////////////

namespace triagens {
  namespace rest {
    class HttpServer;
    class HttpsServer;
    class RestModel;
    class RestServerOptions;

    ////////////////////////////////////////////////////////////////////////////////
    /// @ingroup RestServer
    /// @brief rest server template
    ////////////////////////////////////////////////////////////////////////////////

    class RestServer : public AnyServer {
      public:

        ////////////////////////////////////////////////////////////////////////////////
        /// @brief main without a model
        ////////////////////////////////////////////////////////////////////////////////

        static int main (RestServer* restServer);

        ////////////////////////////////////////////////////////////////////////////////
        /// @brief main with a model
        ////////////////////////////////////////////////////////////////////////////////

        static int main (RestServer* restServer, RestModel* model);

      public:

        ////////////////////////////////////////////////////////////////////////////////
        /// @brief constructor
        ////////////////////////////////////////////////////////////////////////////////

        RestServer (RestServerOptions* options, int argc, char** argv);


      public:

        ////////////////////////////////////////////////////////////////////////////////
        /// @brief number of dispatcher threads to use
        ////////////////////////////////////////////////////////////////////////////////

        size_t getNumberDispatcherThreads () const {
          return _nrDispatcherThreads;
        }


      protected:

        ////////////////////////////////////////////////////////////////////////////////
        /// @brief loads model
        ////////////////////////////////////////////////////////////////////////////////

        bool loadModel ();

        ////////////////////////////////////////////////////////////////////////////////
        /// @brief uses a model
        ////////////////////////////////////////////////////////////////////////////////

        void useModel (RestModel* model);

        ////////////////////////////////////////////////////////////////////////////////
        /// @brief checks additional options
        ////////////////////////////////////////////////////////////////////////////////

        bool checkOptions ();

      protected:

        ////////////////////////////////////////////////////////////////////////////////
        /// {@inheritDoc}
        ////////////////////////////////////////////////////////////////////////////////

        void buildApplicationServer ();

        ////////////////////////////////////////////////////////////////////////////////
        /// {@inheritDoc}
        ////////////////////////////////////////////////////////////////////////////////

        void prepareServer ();

        ////////////////////////////////////////////////////////////////////////////////
        /// {@inheritDoc}
        ////////////////////////////////////////////////////////////////////////////////

        int startupServer ();

      protected:

        ////////////////////////////////////////////////////////////////////////////////
        /// @brief the model to use
        ////////////////////////////////////////////////////////////////////////////////

        RestModel* _model;

        ////////////////////////////////////////////////////////////////////////////////
        /// @brief the various options
        ////////////////////////////////////////////////////////////////////////////////

        RestServerOptions* _options;

        ////////////////////////////////////////////////////////////////////////////////
        /// @brief the http server
        ////////////////////////////////////////////////////////////////////////////////

        HttpServer* _httpServer;


        ////////////////////////////////////////////////////////////////////////////////
        /// @brief the https server
        ////////////////////////////////////////////////////////////////////////////////

        HttpsServer* _httpsServer;



        ////////////////////////////////////////////////////////////////////////////////
        /// @brief number of dispatcher threads
        ////////////////////////////////////////////////////////////////////////////////

        uint32_t _nrDispatcherThreads;


      private:
        int _argc;
        char** _argv;
        string _programName;
        string _shortDescription;
    };
  }
}

#endif
