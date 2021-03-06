#!/bin/sh

if [ $# -lt 2 ]; then
cat <<EOF
Usage: test_ntlm_auth_s3.sh PYTHON SRC3DIR NTLM_AUTH
EOF
exit 1;
fi

PYTHON=$1
SRC3DIR=$2
NTLM_AUTH=$3
shift 3
ADDARGS="$*"

incdir=`dirname $0`/../../../testprogs/blackbox
. $incdir/subunit.sh

failed=0

testit "ntlm_auth" $PYTHON $SRC3DIR/torture/test_ntlm_auth.py $NTLM_AUTH $ADDARGS || failed=`expr $failed + 1`
# This should work even with NTLMv2
testit "ntlm_auth" $PYTHON $SRC3DIR/torture/test_ntlm_auth.py $NTLM_AUTH $ADDARGS --client-domain=fOo --server-domain=fOo || failed=`expr $failed + 1`


testok $0 $failed
