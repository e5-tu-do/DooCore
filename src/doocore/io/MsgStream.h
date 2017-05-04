#ifndef DOOCORE_IO_MSGSTREAM_H
#define DOOCORE_IO_MSGSTREAM_H

// from STL
#include <iostream>
#include <fstream>
#include <sstream> 
#include <cstring>
#include <vector>
#include <set>
#include <unistd.h>

#include "TStopwatch.h"
#include "RooArgSet.h"
#include "TIterator.h"
#include "RooAbsArg.h"
#include "RooRealVar.h"
#include "RooAbsCollection.h"

/**
 * @namespace doocore::io
 * @brief DooCore input/output namespace
 * 
 * This namespace contains all DooCore functionality that deals with 
 * input/output tasks (like MsgStream for terminal output).
 */
namespace doocore {
namespace io {
  
/// Terminal color enum to be used for example in MsgStream class.
enum TerminalColor {
  kTextBlack   = 0,
  kTextRed     = 1,
  kTextGreen   = 2,
  kTextYellow  = 3,
  kTextBlue    = 4,
  kTextMagenta = 5,
  kTextCyan    = 6,
  kTextWhite   = 7,
  kTextNone    = -1
};
  
  /**
   *  @brief Check if terminal is redirected to suppress control characters
   */
  inline bool TerminalIsRedirected() {
    if (!isatty(fileno(stdout))){
      return true;
    } else {
      return false;
    }
  }
  
/*! \class doocore::io::MsgStream 
 * \brief A class for message output using different messages and colors.
 *
 * MsgStream is a handler class to output messages. It can be used like 
 * std::cout and similar streams. The following MsgStream objects are 
 * pre-defined for certain message levels: serr, swarn, sinfo, scfg, sout and 
 * sdebug.
 *
 * Usage example:
 * \code 
 * swarn << "This is a warning message which will be printed in yellow. Value is " << value << endmsg;
 * \endcode
 *
 * The user can define their own MsgStream as well:
 * \code
 * MsgStream mymsgstream(kTextBlue);
 * mymsgstream << "My own stream" << endmsg;
 * \endcode
 */
class MsgStream {
public:
  /**
   *  \brief Constructor for colored output
   *
   *  @param color The color to be used for this stream
   */
  MsgStream(TerminalColor color, const std::string& outfile_name="") : text_color_(color), is_active_(true) {
    if(outfile_name.length()>0){
      filestream_.open(outfile_name.c_str());
    }
  }

  /**
   *  \brief Default constructor for standard uncolored output
   */
  MsgStream() : text_color_(kTextNone), is_active_(true) {}
  
  /**
   *  \brief Get the internal std::ostringstream
   *
   *  This function returns a reference to the std::ostringstream used to store
   *  output before it is flushed to the console.
   *
   *  \return internal std::ostringstream
   */
  std::ostringstream& stream() { return os_; }
  
  /**
   *  \brief Actually output the content of the MsgStream to std::cout. 
   *
   *  Normally not needed as endmsg() will force the output.
   *
   *  \return this MstStream object
   *
   *  \todo Include support for other streams as well besides std::cout.
   */
  MsgStream& doOutput() {
    if (is_active_) {
      if (!TerminalIsRedirected()) SetTerminalColor(text_color_);
      std::cout << std::string(indent_, ' ');
      std::cout << os_.str() << std::endl;
      if (!TerminalIsRedirected()) ResetTerminal();
      if (filestream_.is_open()){
        filestream_ << std::string(indent_, ' ');
        filestream_ << os_.str() << std::endl;
      }
    }
    os_.str("");
      
    return *this;
  }
  
  /**
   *  \brief Stream operator for std::ostream streams. 
   *
   *  Analogous to MsgStream::operator<<(MsgStream& (*)(MsgStream&)). See 
   *  iostreams documentation for reference.
   */
  MsgStream& operator<<(std::ostream& (*_f)(std::ostream&)) {
    _f(os_);
    return *this;
  }
  
  /**
   *  \brief Stream operator for MsgStream streams. 
   *
   *  Analogous to MsgStream::operator<<(std::ostream& (*)(std::ostream&)). See 
   *  iostreams documentation for reference.
   */
  MsgStream& operator<<(MsgStream& (*_f)(MsgStream&)) {
    _f(*this);
    return *this;
  }
  
  /**
   *  \brief Output a horizontal ruler
   */
  void Ruler() {
    for (int i=indent_; i<120; ++i) {
      os_ << "=";
    }
    doOutput();
  }
  
  /**
   *  \brief Set active state of this stream
   *
   *  Determine whether to print messages send to this stream on the console or
   *  not.
   *
   *  @param active_state determines whether printing is enabled or disabled
   */
  void set_active(bool active_state) { is_active_ = active_state; }
  
  /**
   *  \brief Set current indent for new lines
   *
   *  Every new line will be indented by @a indent number of spaces, starting 
   *  after the next endmsg().
   *
   *  @param indent number of spaces to insert at every new line
   */
  void set_indent(int indent) {indent_ = indent;}
  
  /**
   *  \brief Get current indent for new lines
   *
   *  @see MsgStream::set_indent(int)
   *
   *  @return number of spaces to insert at every new line
   */
  int indent() const {return indent_;}
  
  /**
   *  @brief Increment indent for new lines
   *  
   *  @param indent_add additional number of spaces to be used
   */  
  void increment_indent(int indent_add) {indent_ += indent_add;}
  
protected:
  
  /**
   *  @brief Set terminal color
   */
  void SetTerminalColor(TerminalColor color) {
    if (color != kTextNone) {
      printf("%c[%d;%dm",27,1,30+color);
    }
  }
  
  /**
   *  @brief Reset terminal color
   */
  void ResetTerminal() {
    printf("%c[%dm",27,0);
  }
  
  /**
   *  \brief Flush the internal std::ostringstream and output.
   */
  void flush() { os_.flush(); doOutput(); }
  
  /// \brief Internal std::ostringstream for data.
  std::ostringstream os_;            
  /// \brief Text color for output.
  TerminalColor text_color_;
  
  /// \brief determining if stream is active or not (i.e. printing)
  bool is_active_;
  
  /**
   *  \brief Indent for new lines.
   */
  static int indent_;

  /**
   *  \brief Stream for file output.
   */
  std::ofstream filestream_;
};

/// \brief MsgStream function to end a message (i.e. newline) and force the output. 
///
/// Not to be called directly but to be used together with 
/// MsgStream::operator<<() functions.
/// Usage example:
/// \code
/// swarn << "This is a warning message which will be printed in yellow. Value is " << value << endmsg;
/// \endcode
inline MsgStream& endmsg(MsgStream& s) {
  return s.doOutput();
}
  
/// \brief Stream operator for MsgStream streams and additional objects.
/// To be used with other objects like in STL iostreams.
template<typename T>
inline MsgStream& operator<<(MsgStream& lhs, const T& arg) {
  lhs.stream() << arg;
  return lhs;
}

  
/**
 *  @brief Print a vector via MsgStream
 */
template<typename T>
inline MsgStream& operator<<(MsgStream& lhs, const std::vector<T>& arg) {
  if (arg.size() > 0) {
    lhs << "(";
    lhs << arg.front();
    for (typename std::vector<T>::const_iterator it = arg.begin()+1;
         it != arg.end(); ++it) {
      lhs << ", " << *it;
    }
    lhs << ")";
  }
  return lhs;
}

/**
 *  @brief Print a set via MsgStream
 */
template<typename T>
inline MsgStream& operator<<(MsgStream& lhs, const std::set<T>& arg) {
  if (arg.size() > 0) {
    lhs << "(";
    typename std::set<T>::const_iterator it = arg.begin();
    lhs << *it;
    ++it;
    for (;it != arg.end(); ++it) {
      lhs << ", " << *it;
    }
    lhs << ")";
  }
  return lhs;
}

/**
 *  @brief Print a std::pair via MsgStream
 */
template<typename T1, typename T2>
inline MsgStream& operator<<(MsgStream& lhs, const std::pair<T1,T2>& arg) {
  lhs << "(" << arg.first << ", " << arg.second << ")";

  return lhs;
}

  
/**
 *  @brief Function to output bools directly and nicely into MsgStreams
 */
inline MsgStream& operator<<(MsgStream& lhs, bool b) {
  if (b) {
    lhs.stream() << "True";
  } else {
    lhs.stream() << "False";
  }
  return lhs;
}

/**
 *  @brief Function to output TStopwatches directly and nicely into MsgStreams
 */
inline MsgStream& operator<<(MsgStream& lhs, TStopwatch& sw) {
  //sw.Stop();
  
  lhs.stream() << "t(CPU) = " << sw.CpuTime() << " s; t(Real) = " << sw.RealTime() << " s.";
  
  //sw.Reset();
  return lhs;
}

/**
 *  @brief Function to output RooAbsArgs directly and nicely into MsgStreams
 *
 *  This function just prints all RooAbsArgs (including value, error and range if applicable).
 */
template<>
inline MsgStream& operator<< <RooAbsArg>(MsgStream& lhs, const RooAbsArg& arg) {
  using namespace ROOT;
  using namespace RooFit;
  
  const RooRealVar* var_ptr = dynamic_cast<const RooRealVar*>(&arg);
  std::string name = arg.GetName();
  std::string title = arg.GetTitle();
  
  if (var_ptr != nullptr) {
    const RooRealVar& var = *var_ptr;

    lhs.stream() << name;
    if (name != title) { lhs.stream() << " (" << title << ")"; }
    lhs.stream() << " = " << var.getVal();
    if (var.hasAsymError()) {
      lhs.stream() << " (" << var.getAsymErrorLo() << " +" << var.getAsymErrorHi() << ") (+- " <<  var.getError() << ")";
    } else {
      lhs.stream() << " +- " << var.getError();
    }
    if (var.isConstant()) lhs.stream() << " C";
    if (var.hasMin() || var.hasMax()) {
      lhs.stream() << " L(" << var.getMin() << " - " << var.getMax() << ")";
    }
  } else {
    lhs.stream() << name;
    if (name != title) { lhs.stream() << " (" << title << ")"; }
  }
  
  return lhs;
}
  
/**
 *  @brief Function to output RooRealVar directly and nicely into MsgStreams
 *
 *  This function just prints all RooRealVar (including value, error and range if applicable).
 */
template<>
inline MsgStream& operator<< <RooRealVar>(MsgStream& lhs, const RooRealVar& arg) {
  return operator<<(lhs, dynamic_cast<const RooAbsArg&>(arg));
}
  
/**
 *  @brief Function to output RooAbsCollection directly and nicely into MsgStreams
 *
 *  This function just prints all arguments in a RooAbsCollection nicely.
 */
template<>
inline MsgStream& operator<< <RooAbsCollection>(MsgStream& lhs, const RooAbsCollection& argset) {
  using namespace ROOT;
  using namespace RooFit;
  
  bool print_newlines = false;
  if (argset.getSize() >= 10) {
    print_newlines = true;
  }
  
  lhs << "Collection: " << argset.GetName() << " ";
  if (print_newlines) lhs << endmsg;
  
  if (argset.getSize() > 0) {
    //lhs.stream() << "(";
    
    // happy fun time using TIterator, yay!
    TIterator* iter = argset.createIterator();
    const RooAbsArg* arg  = (RooAbsArg*)iter->Next();
    //lhs.stream() << arg->GetName();
    lhs.stream() << arg->GetName();
    
    while ((arg = (const RooAbsArg*)iter->Next())) {
      //lhs.stream() << "," << arg->GetName();
      if (print_newlines) {
        lhs << endmsg;
      } else {
        lhs << ", ";
      }
      //lhs << *arg;
      lhs.stream() << arg->GetName();
    }
    //lhs.stream() << ")";
  }
  
  return lhs;
}
  
/**
 *  @brief Function to output RooArgSet directly and nicely into MsgStreams
 *
 *  This function just prints all arguments in a RooArgSet nicely.
 */
template<>
inline MsgStream& operator<< <RooArgSet>(MsgStream& lhs, const RooArgSet& argset) {
  return operator<<(lhs, dynamic_cast<const RooAbsCollection&>(argset));
}

/**
 *  @brief Function to output RooArgList directly and nicely into MsgStreams
 *
 *  This function just prints all arguments in a RooArgList nicely.
 */
template<>
inline MsgStream& operator<< <RooArgList>(MsgStream& lhs, const RooArgList& argset) {
  return operator<<(lhs, dynamic_cast<const RooAbsCollection&>(argset));
}
  
/// MsgStream for errors. Color: Red
extern MsgStream serr; 
/// MsgStream for warnings. Color: Yellow
extern MsgStream swarn;
/// MsgStream for info messages. Color: Green
extern MsgStream sinfo;
/// MsgStream for config messages. Color: Blue
extern MsgStream scfg;
/// MsgStream for standard messages. Color: None
extern MsgStream sout;
/// MsgStream for debug messages. Color: None
extern MsgStream sdebug;
} // namespace io
} // namespace coocore

#endif // DOOCORE_IO_MSGSTREAM_H
