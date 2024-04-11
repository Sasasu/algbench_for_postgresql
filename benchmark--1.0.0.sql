CREATE SCHEMA b;
CREATE FUNCTION b.run_bench() RETURNS TABLE ("name" text, "pg time" bigint, "cpp time" bigint, "times" float) STRICT AS '$libdir/benchmark.so' LANGUAGE C;
