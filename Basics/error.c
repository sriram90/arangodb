////////////////////////////////////////////////////////////////////////////////
/// @brief error handling
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
/// @author Copyright 2011, triagens GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#include <Basics/Common.h>

#include <Basics/strings.h>
#include <Basics/vector.h>

// -----------------------------------------------------------------------------
// --SECTION--                                                 private variables
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ErrorHandling Error Handling
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief holds the last error
////////////////////////////////////////////////////////////////////////////////

static __thread int ErrorNumber = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief holds the last system error
////////////////////////////////////////////////////////////////////////////////

static __thread int SystemErrorNumber = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief the error messages
////////////////////////////////////////////////////////////////////////////////

static TRI_vector_string_t ErrorMessages;

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                  public functions
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ErrorHandling Error Handling
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the last error
////////////////////////////////////////////////////////////////////////////////

int TRI_errno () {
  return ErrorNumber;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the last error as string
////////////////////////////////////////////////////////////////////////////////

char const* TRI_last_error () {
  if (ErrorNumber == TRI_ERROR_SYS_ERROR) {
    return strerror(SystemErrorNumber);
  }

  if (ErrorNumber < TRI_SizeVectorString(&ErrorMessages)) {
    char const* str = TRI_AtVectorString(&ErrorMessages, ErrorNumber);

    if (str == NULL) {
      return "general error";
    }

    return str;
  }

  return "general error";
}

////////////////////////////////////////////////////////////////////////////////
/// @brief sets the last error
////////////////////////////////////////////////////////////////////////////////

int TRI_set_errno (int error) {
  ErrorNumber = error;

  if (error == TRI_ERROR_SYS_ERROR) {
    SystemErrorNumber = errno;
  }

  return error;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief defines an error string
////////////////////////////////////////////////////////////////////////////////

void TRI_set_errno_string (int error, char const* msg) {
  if (error >= TRI_SizeVectorString(&ErrorMessages)) {
    TRI_ResizeVectorString(&ErrorMessages, error + 1);
  }

  TRI_SetVectorString(&ErrorMessages, error, TRI_DuplicateString(msg));
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                            MODULE
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// --SECTION--                                                  public functions
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ErrorHandling Error Handling
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief initialises the error messages
////////////////////////////////////////////////////////////////////////////////

void TRI_InitialiseError () {
  TRI_InitVectorString(&ErrorMessages);

  TRI_set_errno_string(0, "no error");
  TRI_set_errno_string(1, "failed");
  TRI_set_errno_string(2, "system error");
}

////////////////////////////////////////////////////////////////////////////////
/// @brief shuts down the error messages
////////////////////////////////////////////////////////////////////////////////

void TRI_ShutdownError () {
  TRI_DestroyVectorString(&ErrorMessages);
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// Local Variables:
// mode: outline-minor
// outline-regexp: "^\\(/// @brief\\|/// @addtogroup\\|// --SECTION--\\)"
// End:
