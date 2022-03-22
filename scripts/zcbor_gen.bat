
rem Generate C files from cddl
rem Run from /scripts/

SET c="zcbor_mgmt.cddl"
rem create decode only
SET both=-c ..\zcbor_mgmt.cddl --default-max-qty 0 code -d -t
SET oc=..\src
SET oh=..\include

rem At this time everything is command/response

FOR %%x IN (error_rsp) DO zcbor %both% %%x --oc %oc%\%%x.c --oh %oh%\%%x.h

