#include "doocore/io/MsgStream.h"

namespace doocore {
namespace io {
int MsgStream::indent_ = 0;

MsgStream serr   (doocore::lutils::kTextRed);
MsgStream swarn  (doocore::lutils::kTextYellow);
MsgStream sinfo  (doocore::lutils::kTextGreen);
MsgStream scfg   (doocore::lutils::kTextBlue);
MsgStream sout   (doocore::lutils::kTextNone);
MsgStream sdebug (doocore::lutils::kTextNone);
} // namespace utils
} // namespace doofit