    if ( b_containsDataEventLabel == TRUE )
        i_MetaInfo	+= 2;

    if ( ( b_containsDataDate == TRUE ) && ( b_containsDataTime == TRUE ) )
        i_MetaInfo	+= 4;

    if ( ( b_containsDataLatitude == TRUE ) && ( b_containsDataLongitude == TRUE ) )
        i_MetaInfo	+= 8;

// **********************************************************************************************
// check Position

    if ( i_MetaInfo < 8 )
    {
        if ( ( b_containsLatitude == FALSE ) || ( b_containsLongitude == FALSE ) )
        {
            if ( ( i_OutputFormat != FORMAT_UNFORMATEDTEXTFILE ) && ( i_OutputFormat != FORMAT_FORMATEDTEXTFILE ) )
                err -= 40;

            if ( gb_setPosition == FALSE )
            {
                if ( b_containsLatitude == FALSE )
                    i_MetaInfo	+= 16;

                if ( b_containsLongitude == FALSE )
                    i_MetaInfo	+= 32;
            }
            else
            {
                if ( b_containsLatitude == FALSE )
                    s_Latitude = QString( "%1" ).arg( (float) rand()/(float) RAND_MAX ) ;

                if ( b_containsLongitude == FALSE )
                    s_Longitude = QString( "%1" ).arg( (float) rand()/(float) RAND_MAX ) ;
            }
        }
    }
