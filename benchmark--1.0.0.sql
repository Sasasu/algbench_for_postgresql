CREATE SCHEMA b;
CREATE FUNCTION b.run_bench() RETURNS TABLE ("name" text, "pg time" int, "cpp time" int) STRICT AS '$libdir/benchmark.so' LANGUAGE C;
