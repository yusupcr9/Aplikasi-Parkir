setInterval(function(){
    $(document).ready(function() {
        $.ajax({
            url: "/bioparks/data-riwayat-in-out",
            dataType: 'json',
            success: function(result){
                if(result['riwayats']){
                    var html_incident = '';
                    var number = 1;
                    var jam_keluar = '';
                    $.each(result['riwayats'], function(i, data) {
                        if (data.jam_keluar){
                            jam_keluar = data.jam_keluar
                        }
                        else {
                            jam_keluar =''
                        }
                        riwayats = 'riwayats'
                        html_incident += '<tr><td>'+number+'</td><td>'+data.name+'</td><td>'+data.jam_masuk+'</td><td>'+jam_keluar+'</td></tr>';
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
        $.ajax({
            url: "/bioparks/data-monitoring",
            dataType: 'json',
            success: function(result){
                if(result['monitor']){
                    var html_incident = '';
                    var number = 1;
                    $.each(result['monitor'], function(i, data) {
                        html_incident += '<div class="col-3 text-center"><h4>Masuk: '+data.masuk+'</h4></div><div class="col-3 text-center"><h4>Keluar: '+data.keluar+'</h4></div><div class="col-3 text-center"><h4>Parkir: '+data.parkir+'</h4></div><div class="col-3 text-center"><h4>Sisa Slot: '+data.kuota+'</h4></div>';
                        number += 1
                    });
                    $("#monitor").html(html_incident);
                }
                console.log(result)
            },
            error: function(){
                alert('tidak bisa ambil data');
            }
        });
    } );
}, 500);

