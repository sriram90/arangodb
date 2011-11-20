////////////////////////////////////////////////////////////////////////////////
/// @brief application server dispatcher implementation
///
/// @file
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
/// @author Copyright 2009-2011, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef FYN_APPLICATIONSERVER_APPLICATION_SERVER_DISPATCHER_IMPL_H
#define FYN_APPLICATIONSERVER_APPLICATION_SERVER_DISPATCHER_IMPL_H 1

#include "ApplicationServer/ApplicationServerSchedulerImpl.h"

#include <Rest/ApplicationServerDispatcher.h>
#include <Rest/Dispatcher.h>

namespace triagens {
  namespace rest {

    ////////////////////////////////////////////////////////////////////////////////
    /// @brief application server dispatcher implementation
    ////////////////////////////////////////////////////////////////////////////////

    class ApplicationServerDispatcherImpl : virtual public ApplicationServerSchedulerImpl,
                                            virtual public ApplicationServerDispatcher {
      public:

        ////////////////////////////////////////////////////////////////////////////////
        /// @brief constructor
        ////////////////////////////////////////////////////////////////////////////////

        ApplicationServerDispatcherImpl (string const& description, string const& version);

        ////////////////////////////////////////////////////////////////////////////////
        /// @brief destructor
        ////////////////////////////////////////////////////////////////////////////////

        ~ApplicationServerDispatcherImpl ();

      public:

        ////////////////////////////////////////////////////////////////////////////////
        /// {@inheritDoc}
        ////////////////////////////////////////////////////////////////////////////////

        Dispatcher* dispatcher () const {
          return _dispatcher;
        }

        ////////////////////////////////////////////////////////////////////////////////
        /// {@inheritDoc}
        ////////////////////////////////////////////////////////////////////////////////

        void buildDispatcher ();

        ////////////////////////////////////////////////////////////////////////////////
        /// {@inheritDoc}
        ////////////////////////////////////////////////////////////////////////////////

        void buildDispatcherReporter ();

        ////////////////////////////////////////////////////////////////////////////////
        /// {@inheritDoc}
        ////////////////////////////////////////////////////////////////////////////////

        void buildStandardQueue (size_t nrThreads);

        ////////////////////////////////////////////////////////////////////////////////
        /// {@inheritDoc}
        ////////////////////////////////////////////////////////////////////////////////

        void buildNamedQueue (string const& name, size_t nrThreads);

      public:

        ////////////////////////////////////////////////////////////////////////////////
        /// {@inheritDoc}
        ////////////////////////////////////////////////////////////////////////////////

        void start ();

        ////////////////////////////////////////////////////////////////////////////////
        /// {@inheritDoc}
        ////////////////////////////////////////////////////////////////////////////////

        void wait ();

        ////////////////////////////////////////////////////////////////////////////////
        /// {@inheritDoc}
        ////////////////////////////////////////////////////////////////////////////////

        void beginShutdown ();

        ////////////////////////////////////////////////////////////////////////////////
        /// {@inheritDoc}
        ////////////////////////////////////////////////////////////////////////////////

        void shutdown ();

      protected:

        ////////////////////////////////////////////////////////////////////////////////
        /// @brief scheduler
        ////////////////////////////////////////////////////////////////////////////////

        Dispatcher* _dispatcher;

        ////////////////////////////////////////////////////////////////////////////////
        /// @brief reporting task
        ////////////////////////////////////////////////////////////////////////////////

        Task* _dispatcherReporterTask;
    };
  }
}

#endif
