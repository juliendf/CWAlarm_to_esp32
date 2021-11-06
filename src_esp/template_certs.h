#include <pgmspace.h>

// Amazon Root CA 1
static const char AWS_PUBLIC_CERT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
<INSERT_outputs/certificates/AmazonRootCA1.pem>
-----END CERTIFICATE-----
)EOF";

// Device Certificate
static const char AWS_DEVICE_CERT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
<INSERT_outputs/certificates/${THING_NAME}-certificate.pem.crt>
-----END CERTIFICATE-----
)KEY";

// Device Private Key
static const char AWS_PRIVATE_KEY[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
<INSERT_outputs/certificates/${THING_NAME}-private.pem.key>
-----END RSA PRIVATE KEY-----
)KEY";