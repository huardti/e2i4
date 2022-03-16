<?xml version="1.0" encoding="windows-1250"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html" encoding="windows-1250"/>

<xsl:template match="project">
  <html>
    <head>
      <title>
        <xsl:value-of select="name"/>
      </title>
    </head>
    <body>
      <h1>
        <xsl:value-of select="name"/>
      </h1>
      <h3>
        <xsl:value-of select="description"/>
      </h3>
      <p>
        <xsl:value-of select="copyright"/>
      </p>
      <xsl:for-each select="update">
          <hr/>
        [<xsl:value-of select="./version"/>]
        <xsl:value-of select="./date"/>
          <xsl:for-each select="point">
            <li>
              <xsl:value-of select="."/>
            </li>
          </xsl:for-each>
        </xsl:for-each>
      </body>
    </html>
  </xsl:template>
</xsl:stylesheet>