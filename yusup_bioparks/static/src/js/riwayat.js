// $(document).ready(function() {
//     $('#riwayat').DataTable( {
//         "ajax": '/bioparks/data-riwayat',
//         "responsive": true,
//         "paging":   false,
//         "ordering": false,
//         "info":     false,
//         "wrapper": false
//     } );
// } );
setInterval(function(){
    $(document).ready(function() {
        $.ajax({
            url: "/bioparks/data-riwayat",
            dataType: 'json',
            success: function(result){
                if(result['riwayats']){
                    var html_incident = '';
                    var number = 1;
                    $.each(result['riwayats'], function(i, data) {
                        riwayats = 'riwayats'
                        html_incident += '<tr><td>'+number+'</td><td>'+data.name+'</td><td>'+data.jam_keluar+'</td></tr>';
                        number += 1
                    });
                    $("#body").html(html_incident);
                }
                console.log(result)
            },
            error: function(){
                alert('tidak bisa ambil data');
            }
        });
        
    } );
}, 500);