#!/usr/bin/python
# Python script

import os
import urlparse


print 'Python Test Script'
print 'Environment variables:'
print 'WEB_ROOT:       ', os.environ.get('WEB_ROOT')
print 'REQUEST_METHOD: ', os.environ.get('REQUEST_METHOD')
print 'CGI_BIN:        ', os.environ.get('CGI_BIN')
print 'UTIL:           ', os.environ.get('UTIL')
print 'QUERY_STRING:   ', os.environ.get('QUERY_STRING')
print '.....'

queryString = os.getenv('QUERY_STRING', '<<none>>')

if queryString != '<<none>>':
    parameters = urlparse.parse_qs(queryString)
    for key,value in parameters.iteritems():
        print key, ' = ', value
    
print '[Done]'
