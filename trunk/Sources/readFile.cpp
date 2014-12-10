    QFile fpdb( s_FilenamePDB );
    if ( !fpdb.open( QIODevice::ReadOnly ) )
        return( -101 );

    QByteArray ba = fpdb.readAll();

    fpdb.close();

    sl_Input = QString( ba ).split( "\n" );

