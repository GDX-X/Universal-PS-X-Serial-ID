# Universal-PS-X-Serial-ID
Get PS1 Serial ID From BIN, VCD, PBP, PKG

- [x] Support .BIN, .VCD, .PBP, .PKG
- [x] Support LSP Serial ID
- [X] Support Some exeception games don"t have system.cnf

```
UPSX_SID "filename"  # Output original Serial ID found

UPSX_SID "filename" -1   # Force format output SLES_123.45
UPSX_SID "filename" -2   # Force format output SLES-12345
UPSX_SID "filename" -3   # Force format output SLES12345
```

