<?xml version="1.0" encoding="UTF-8"?>
<sqls namespace="PLAN_INFO">

<define id="fields">PLAN_ID, PLAN_NUMBER, PLAN_NAME</define>

<sql id="queryAll">
SELECT <include refId="fields"/> FROM plan_info
</sql>

<sql id="queryByTime">
SELECT <include refId="fields"/> FORM plan_info WHERE PLAN_ID BETWEEN %1 AND %2 ORDER BY PLAN_ID asc
</sql>

<sql id="queryById">
SELECT <include refId="fields"/> FROM plan_info WHERE PLAN_ID = %1 ORDER BY PLAN_ID asc
</sql>

<sql id="insert">
INSERT INTO plan_info (<include refId="fields"/>) VALUES(:PLAN_ID, :PLAN_NUMBER, :PLAN_NAM)
</sql>

<sql id="update">
UPDATE plan_info SET PLAN_ID=:PLAN_ID, PLAN_NUMBER=:PLAN_NUMBER, PLAN_NAM=:PLAN_NAM
</sql>

</sqls>
