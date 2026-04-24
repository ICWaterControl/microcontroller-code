#ifndef AWS_IOT_CONFIG_H
#define AWS_IOT_CONFIG_H

// Substitua pelos valores do seu AWS IoT Core.
// O endpoint pode ser encontrado em AWS IoT Core > Settings.
static const char AWS_IOT_ENDPOINT[] = "SEU_ENDPOINT_DO_AWS_IOT_CORE";

// Client ID MQTT permitido na policy enviada pelo usuario.
static const char AWS_IOT_CLIENT_ID[] = "basicPubSub";

// Certificado raiz da AWS IoT (Amazon Root CA 1).
static const char AWS_IOT_ROOT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
COLE_AQUI_O_AMAZON_ROOT_CA_1
-----END CERTIFICATE-----
)EOF";

// Certificado X.509 do dispositivo ESP32 gerado no AWS IoT Core.
static const char AWS_IOT_DEVICE_CERT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
COLE_AQUI_O_CERTIFICADO_X509_DO_DISPOSITIVO
-----END CERTIFICATE-----
)EOF";

// Chave privada do dispositivo ESP32 gerada no AWS IoT Core.
static const char AWS_IOT_PRIVATE_KEY[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEAx7MYpa1qzIIZmjXu05aMbHg8lZiRsJVTa6ZX73NsrhHdg6xv
Aps7Ckm4uHqbSBk0JfVsOPUpntZf6iNfARwEBr+VD9KhbtgerqGyoG851XHgn2nM
K1RETYdgBEp9ueegK/p6yVzpWEeuaqlvh0r/o4GXz7oqMyhk524Hu9mOz3R6ttxk
qHALR6ne2mTfJDYuMuil1OvumL/sEYupra5sVUwfO6e4tyzJB8+OUab7dHGU72de
dcvKN62TG3vVSJOtiwLox1sTPiuEtAqUoU9CVqMnrUC9ngZzeiZjpBtY/FZ+sRGQ
X8Ic8fT5AOugj9J9yY2++0ylkpVUuo5CH6anlQIDAQABAoIBAExo5qFd/WNK1GkB
AeTL0ecfCm9i9TvPpx+lgjgIvijMhSmArOkeZDTHiHptz0SDAEUV5tbUV5yDAigg
aJWQFqpOW8a2HTbAUt0W2bYxWLum2v+kB478qzXO+poptnmPeuXY7WpmtpIku0Ps
mNP3imERkPjWjpcMea9jlmn2EIKnrdTDBstDTFAWQMXrguQlboasTB9dAST9vpx7
uLmiIeXoPXKPzg+HK4pWysJSidvEl2ll0YQPDKoIZT7JZ3R7jHTHha94Jf65de62
Y3JCubdyDQghFeJRmm4uNg/bHn6KdETKThlVh5I7GeIXxPEcmldz1OD0oUKgnpkq
fg9ntfECgYEA40WyGHsIQ0guvOiDR74MNQ1aHb9r+90NfImoov3hiDG62fngaQgh
DcgUz8pUHB+9PbjZ6OX06pDvzcYehg90fPKZ+S3cpgcwNtlVvFEMybf6OOG/OvEq
jQwBTr0f4x7iJiIPf8tNWzvNJscMjUttiNt41XVUhpDUlSeyspD5MT8CgYEA4PEt
SHqQVeirGx+dTjmG7d/8BloXsGU3z0SlraaVRwsA1zdvLl5Xc1zH4dcajtHh1xc4
CqzeLQY0aAgoXQGi1cXmud6xITsSH+mGzQlDxzTlu4YQNhK/638A5grdbAwpOFjK
6Cf1gL5/5ymkhshmovjMoR0Bcy1Yr6EjmmZ6HisCgYAb2MhPk5OZmtiPMPYU7kZU
ocQu7OtwOs+SONO+1NizdVRtrN5AQEy8gY17QFE5+PGZBHM+xB5UN1oKnoHS70XY
oWkYTPRCK5NOxSZqIowskyxHl/F0aWRLT/NBEI89puUSww2fHOvnh9ZFMBJrpZhQ
MoQFs5CdxFUnyWtt0bGWxQKBgBbnV7YGjMXCkBGN8YVyhBOad93ptHnIhJs9ISWC
TbbTHv3K7i4pevwLGWscbN7X7gfpnFIx8dlhxVpGYl29+eeMZAbO7vg0PI81Rv1/
kQAqma2zY9ktpY8D+oJx3sPOpS/QkrB6yRuXygwuiEsVAqZbcPKCspAYsOwJG0fw
YUTLAoGBALQhttFRYAZsJRI7grZmTQiWmmrACA7gUyaSGfuya79cWFNY+HV2H5bq
auj4dzw3uPuk0UafXcS9q3ZrSoTlUb2wX7F72zIoiKTpiIxNt8RBVmUaz0xgG/Gn
iNdTTDon4SehAnW/FDSqUd5j8MktgL+aMLAonA8BsBfEZhFzmMvE
-----END RSA PRIVATE KEY-----
)EOF";

#endif