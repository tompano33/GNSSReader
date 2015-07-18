<?xml version="1.0" encoding="utf-8"?>  

<xsl:stylesheet version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
xmlns:xs="http://www.w3.org/2001/XMLSchema" exclude-result-prefixes="xs">

  <xsl:template match="metadata">
    <html>
      <head>
        <style type="text/css">
          .tblhdr
          {
          background-color: #CCCCFF;
          }
        </style>
      </head>
      <body>

        <!-- REPORT DEFINED DATAFILES -->
        <xsl:if test ="datafile">
          <xsl:for-each select="datafile">
            <h2>Data File: <xsl:apply-templates select="url" /></h2>
            <table>
              <tr><td class="tblhdr">Owner</td><td><xsl:value-of select="owner"/></td></tr>
              <tr><td class="tblhdr">Copyright</td><td><xsl:value-of select="copyright"/></td></tr>
              <tr><td class="tblhdr">Created</td>
                <td>
                  <xsl:variable name="dt" as="xs:dateTime" select="xs:dateTime('2012-10-21T22:10:15')"/>
                  <xsl:value-of select="format-dateTime($dt, '[D] [MNn] [Y] [h]:[m01][PN,*-2]')"/>
                </td>
              </tr>
              <tr><td class="tblhdr">Modified</td>
                <td>
                  <xsl:variable name="dt" as="xs:dateTime" select="xs:dateTime('2012-10-21T22:10:15')"/>
                  <xsl:value-of select="format-dateTime($dt, '[D] [MNn] [Y] [h]:[m01][PN,*-2]')"/>
                </td>
              </tr>
              <tr><td class="tblhdr">Next File</td><td><xsl:value-of select="next"/></td></tr>
              <tr><td class="tblhdr">Previous File</td><td><xsl:value-of select="previous"/></td></tr>
              <tr><td class="tblhdr">DataOffset</td><td><xsl:value-of select="offset"/> bytes</td></tr>
              <tr><td class="tblhdr">Base Frequency</td><td><xsl:apply-templates select="rate"/></td></tr>
            </table>

            <xsl:call-template name ="ArtifactsComments"/>
            
            <h3>Streams</h3>
            <table>
              <tr >
                <th class="tblhdr">ID</th>
                <th class="tblhdr">Channel</th>
                <th class="tblhdr">Rate Factor</th>
                <th class="tblhdr">Format</th>
                <th class="tblhdr">Quantization</th>
                <th class="tblhdr">Packed Bits</th>
                <th class="tblhdr">Alignment</th>
                <th class="tblhdr">Encoding</th>
                <th class="tblhdr">Delay Bias</th>
              </tr>
              <xsl:apply-templates select="stream"/>
            </table>
          </xsl:for-each>


          
          <hr/>
        </xsl:if>

        <!-- REPORT CHANNEL SPECIFICATIONS -->
        <xsl:if test="datafile/stream/channel or channel">
          <h2>Channel Specifications</h2>
          <xsl:apply-templates select="datafile/stream/channel"/>
          <xsl:apply-templates select="channel"/>
          <hr/>
        </xsl:if>

        <!-- REPORT GLOBAL STREAM SPECIFICATIONS -->
        <xsl:if test="stream">
          <h2>Global Stream Specifications</h2>
          <table>
            <tr bgcolor="#9acd32">
              <th class="tblhdr">ID</th>
              <th class="tblhdr">Channel</th>
              <th class="tblhdr">Rate Factor</th>
              <th class="tblhdr">Format</th>
              <th class="tblhdr">Quantization</th>
              <th class="tblhdr">Packed Bits</th>
              <th class="tblhdr">Alignment</th>
              <th class="tblhdr">Encoding</th>
              <th class="tblhdr">Delay Bias</th>
            </tr>
            
            <xsl:apply-templates select="stream"/>
          </table>

          <xsl:apply-templates select="frame"/>
          <xsl:apply-templates select="subframe"/>
          <hr/>
        </xsl:if>

        <!-- REPORT SESSION SPECIFICATIONS-->
        <xsl:if test="session or datafile/session">
          <h2>Session Specifications</h2>
          <xsl:apply-templates select="datafile/session"/>
          <xsl:apply-templates select="session"/>
          <hr/>
        </xsl:if>

        <!-- REPORT SYSTEM SPECIFICATIONS-->
        <xsl:if test="system or channel/system or stream/channel/system or datafile/stream/channel/system">
          <h2>System Specifications</h2>
          <xsl:apply-templates select="datafile/stream/channel/system"/>
          <xsl:apply-templates select="stream/channel/system"/>
          <xsl:apply-templates select="channel/system"/>
          <xsl:apply-templates select="system"/>
          <hr/>
        </xsl:if>
      </body>
    </html>
  </xsl:template>

  
  <xsl:template match="stream">
      <tr>
        <td><xsl:value-of select="@id"/></td>
        <td><xsl:value-of select="channel/@id"/></td>
        <td><xsl:value-of select="ratefactor"/></td>
        <td><xsl:value-of select="format"/></td>
        <td><xsl:value-of select="quantization"/></td>
        <td><xsl:value-of select="packedbits"/></td>
        <td><xsl:value-of select="alignment"/></td>
        <td><xsl:value-of select="encoding"/></td>
        <td><xsl:apply-templates select="delaybias"/></td>
      </tr>
  </xsl:template>

  <xsl:template match="channel">
    <xsl:if test="*">    
    <h3><xsl:value-of select="@id"/></h3>
    <table>
        <tr><td class="tblhdr">Parent</td><td><xsl:value-of select="../@id"/></td></tr>
        <tr><td class="tblhdr">System</td><td><xsl:value-of select="system/@id"/></td></tr>
        <tr><td class="tblhdr">Center Frequency</td><td><xsl:apply-templates select="centerfreq"/></td></tr>
        <tr><td class="tblhdr">Translated Frequency</td><td><xsl:apply-templates select="translatedfreq"/></td></tr>
        <tr><td class="tblhdr">Inverted</td><td><xsl:value-of select="inverted"/></td></tr>
        <tr><td class="tblhdr">Delay Bias</td><td><xsl:value-of select="delaybias"/></td></tr>
    </table>
    <xsl:call-template name ="ArtifactsComments"/>
    </xsl:if>      
  </xsl:template>

  <xsl:template match="session">
    <xsl:if test="*">    
    <h3>Session&#160;<xsl:value-of select="@id"/></h3>
    <table>
      <tr><td class="tblhdr">Scenario</td><td><xsl:value-of select="scenario"/></td></tr>
      <tr><td class="tblhdr">Campaign</td><td><xsl:value-of select="campaign"/></td></tr>
      <tr><td class="tblhdr">Location</td><td><xsl:value-of select="location"/></td></tr>
      <tr><td class="tblhdr">Contact</td><td><xsl:value-of select="contact"/></td></tr>
    </table>
    <xsl:call-template name ="ArtifactsComments"/>
    </xsl:if>
  </xsl:template>

  <xsl:template match="system">
    <xsl:if test="*">
    <h3><xsl:value-of select="@id"/>&#160;<xsl:value-of select="@type"/></h3>
    <table>
      <tr><td class="tblhdr">Model</td><td><xsl:value-of select="model"/></td></tr>
      <tr><td class="tblhdr">RF Configuration</td><td><xsl:value-of select="rfconfig"/></td></tr>
      <tr><td class="tblhdr">Oscillator</td><td><xsl:value-of select="oscillator"/></td></tr>
      <tr><td class="tblhdr">Parent</td><td><xsl:value-of select="../@id"/></td></tr>
    </table>
    <xsl:call-template name ="ArtifactsComments"/>
    </xsl:if>
  </xsl:template>


  <!-- Template reports any artifacts or comments defined -->
  <xsl:template name="ArtifactsComments">
    <!-- Display any Comments-->
    <xsl:if test ="comment">
      <h4>Comments</h4>
        <xsl:for-each select="comment">
          <p><xsl:value-of select="."/></p>
        </xsl:for-each>
      </xsl:if>

    <!-- Display any artifacts -->
    <xsl:if test ="artifact">
      <h4>Related Artifacts</h4>
      <ul>
        <xsl:for-each select="artifact">
          <li>
            <xsl:apply-templates select="."/>
          </li>
        </xsl:for-each>
      </ul>
    </xsl:if>
  </xsl:template>


  <!-- Template frames -->
  <xsl:template match="frame">
      <h4>Frame Specification</h4>
    <table>
      <tr><td class="tblhdr">Size Header</td><td><xsl:value-of select="@sizeHeader"/></td></tr>
      <tr><td class="tblhdr">Size Footer</td><td><xsl:value-of select="@sizeFooter"/></td></tr>
      <tr><td class="tblhdr">Count Subframes</td><td><xsl:value-of select="@count"/></td></tr>
    </table>
  </xsl:template>

  <!-- Template frames -->
  <xsl:template match="subframe">
      <h4>Subframe Specification</h4>
    <table>
      <tr><td class="tblhdr">Sample Word Count </td><td><xsl:value-of select="@words"/></td></tr>
      <tr><td class="tblhdr">Subframe Size (bytes)</td><td><xsl:value-of select="@size"/></td></tr>
      <tr><td class="tblhdr">Endian Ordering</td><td><xsl:value-of select="@endian"/></td></tr>
      <tr><td class="tblhdr">Sample Alignment</td><td><xsl:value-of select="@alignment"/></td></tr>
    </table>
  </xsl:template>

  <xsl:template match="centerfreq">
    <xsl:call-template name="frequency"/>
  </xsl:template>
  <xsl:template match="translatedfreq">
    <xsl:call-template name="frequency"/>
  </xsl:template>
  <xsl:template match="rate">
    <xsl:call-template name="frequency"/>
  </xsl:template>
  <xsl:template match="delaybias">
    <xsl:call-template name="duration"/>
  </xsl:template>

  <xsl:template match="artifact">
    <A TARGET="_blank">
      <xsl:attribute name="HREF">
        <xsl:apply-templates/>
      </xsl:attribute>
      <xsl:apply-templates/>
    </A>
  </xsl:template>

  <xsl:template match="url">
    <A TARGET="_blank">
      <xsl:attribute name="HREF">
        <xsl:apply-templates/>
      </xsl:attribute>
      <xsl:apply-templates/>
    </A>
  </xsl:template>

  <xsl:template name="frequency">
    <xsl:choose>
      <xsl:when test="@format">
        <xsl:value-of select="self::*"/>&#160;<xsl:value-of select="@format"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="self::*"/>&#160;Hz
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  <xsl:template name="duration">
    <xsl:choose>
      <xsl:when test="@format">
        <xsl:value-of select="self::*"/>&#160;<xsl:value-of select="@format"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="self::*"/>&#160;sec
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>
</xsl:stylesheet>
