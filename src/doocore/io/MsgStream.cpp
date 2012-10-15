#include "doocore/io/MsgStream.h"

namespace doocore {
namespace io {
int MsgStream::indent_ = 0;

MsgStream serr   (kTextRed);
MsgStream swarn  (kTextYellow);
MsgStream sinfo  (kTextGreen);
MsgStream scfg   (kTextBlue);
MsgStream sout   (kTextNone);
MsgStream sdebug (kTextNone);
} // namespace utils
} // namespace doofit