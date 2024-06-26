### benchmark the fundamental data structure performance inside PostgreSQL

```
> create extension benchmark;
> select * from b.run_bench();
+------------------------+----------+----------+-------------------------+
| name                   | pg time  | cpp time | cpp is X faster than pg |
|------------------------+----------+----------+-------------------------|
| bitmapset_compare      | 2045836  | 92854    | 22.0328257264092        |
| bitmapset_is_subset    | 1682094  | 707978   | 2.3759128108500547      |
| bitmapset_add_member   | 11280881 | 3356784  | 3.360621654536008       |
| list_sort              | 9482300  | 3540849  | 2.6779735594485956      |
| list_iter              | 11612203 | 11354820 | 1.0226672901904212      |
| list_append            | 39379767 | 17319444 | 2.273731593231284       |
| hash_set_alloc         | 747221   | 162665   | 4.5936187870777365      |
| hash_set_noalloc       | 994525   | 147136   | 6.759222759895607       |
| hash_get_rand          | 374352   | 13305    | 28.13618940248027       |
| hash_get_scan          | 155852   | 10970    | 14.20711030082042       |
| hash_delete            | 339366   | 88686    | 3.8266017184222987      |
| functioncall_1         | 53470    | 541      | 98.8354898336414        |
| functioncall_2         | 66254    | 2255     | 29.380931263858095      |
| functioncall_7         | 120676   | 2275     | 53.0443956043956        |
| functioncall_9         | 142918   | 2264     | 63.12632508833922       |
| memoryalloc_small      | 6227567  | 4749616  | 1.3111727348063507      |
| memoryalloc_big        | 10944420 | 11732499 | 0.9328293997723759      |
| memoryalloc_free       | 7681032  | 1586595  | 4.841205222504798       |
| memoryalloc_free_batch | 4797425  | 79990    | 59.97530941367671       |
| sort_qsort             | 1203086  | 115046   | 10.457434417537334      |
| sort_cppsort           | 1076839  | 35447    | 30.378847293141874      |
+------------------------+----------+----------+-------------------------+
```
