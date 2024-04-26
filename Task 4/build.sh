rm -r src/*
rm -r build/*
mkdir src
mkdir build
asn1c description.asn1 -D src -no-gen-OER -no-gen-example -fcompound-names -fno-include-deps
gcc -Isrc -o build/client client.c src/*.c -lsctp -DPDU=RRC -DASN_DISABLE_OER_SUPPORT
gcc -Isrc -o build/server server.c src/*.c -lsctp -DPDU=RRCConnectionSetupComplete -DASN_DISABLE_OER_SUPPORT