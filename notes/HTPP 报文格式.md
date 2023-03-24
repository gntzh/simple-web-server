# 请求

```
<Method> <Request-URI> <HTTP-Version><CR><LF>
<header1_name>: <header1_value><CR><LF>
<header2_name>: <header2_value><CR><LF>
<header3_name>: <header3_value><CR><LF>
<CR>|<LF>
<message-body>
```

例如
```
GET / HTTP/1.1
```

```
GET /index.html HTTP/1.1
Host: example.com
Accept-Language: en-us
Accept-Encoding: gzip, deflate
```

# 响应

```
<HTTP-Version> <Status-Code> <Reason-Phrase><CR><LF>
<headers><CR><LF>
<message-body>
```
