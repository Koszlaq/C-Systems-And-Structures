#include "file_reader.h"

struct disk_t* disk_open_from_file(const char* volume_file_name){
    if (volume_file_name==NULL){
        errno=EFAULT;
        return NULL;
    }

    FILE* plik=fopen(volume_file_name, "rb");
    if (plik==NULL) {
        errno=ENOENT;
        return NULL;
    }

    struct disk_t* dysk=malloc(sizeof(struct disk_t));
    if (dysk==NULL) {
        fclose(plik);
        errno=ENOMEM;
        return NULL;
    }

    dysk->dysk=plik;

    return dysk;
}

int disk_read(struct disk_t* pdisk, int32_t first_sector, void* buffer, int32_t sectors_to_read){
    if(pdisk==NULL||buffer==NULL){
        errno=EFAULT;
        return -1;
    }

    fseek(pdisk->dysk,first_sector*ROZMIAR_SEKTORA,SEEK_SET);
    unsigned long wynik=fread(buffer,ROZMIAR_SEKTORA,sectors_to_read,pdisk->dysk);
    if((int32_t)wynik!=sectors_to_read){
        errno=ERANGE;
        return -1;
    }

    return (int)wynik;
}

int disk_close(struct disk_t* pdisk){
    if (pdisk==NULL) {
        errno=EFAULT;
        return -1;
    }

    fclose(pdisk->dysk);
    free(pdisk);

    return 0;
}

struct volume_t* fat_open(struct disk_t* pdisk, uint32_t first_sector){
    if(pdisk==NULL||pdisk->dysk==NULL){
        errno=EFAULT;
        return NULL;
    }

    struct volume_t *FAT=malloc(sizeof(struct volume_t));
    if(FAT==NULL){
        errno=ENOMEM;
        return NULL;
    }

    FAT->dysk=pdisk;
    if(disk_read(pdisk,first_sector,&FAT->boot,1)==-1 || FAT->boot.signature_value!=0xaa55){
        errno=EINVAL;
        free(FAT);
        return NULL;
    }

    FAT->FAT1=malloc(FAT->boot.bitow_na_sektor*FAT->boot.FAT_rozmiar);
    if(FAT->FAT1==NULL){
        errno = ENOMEM;
        free(FAT);
        return NULL;
    }
    FAT->FAT2=malloc(FAT->boot.bitow_na_sektor*FAT->boot.FAT_rozmiar);
    if(FAT->FAT2==NULL){
        errno=ENOMEM;
        free(FAT->FAT1);
        free(FAT);
        return NULL;
    }

    if(disk_read(pdisk,FAT->boot.zarezerwowane_sektory,FAT->FAT1,FAT->boot.FAT_rozmiar)==-1 || disk_read(pdisk,(int32_t)FAT->boot.zarezerwowane_sektory+FAT->boot.FAT_rozmiar,FAT->FAT2,FAT->boot.FAT_rozmiar)==-1){
        errno=EINVAL;
        free(FAT->FAT2);
        free(FAT->FAT1);
        free(FAT);
        return NULL;
    }

    for(int i=0;i<(int)FAT->boot.bitow_na_sektor*FAT->boot.FAT_rozmiar;i++) {
        if(*((char *)FAT->FAT1+i)!=*((char *)FAT->FAT2+i)) {
            errno=EINVAL;
            free(FAT->FAT2);
            free(FAT->FAT1);
            free(FAT);
            return NULL;
        }
    }

    int32_t root_sektory=(FAT->boot.root_ile*32+(FAT->boot.bitow_na_sektor-1))/FAT->boot.bitow_na_sektor;
    int32_t pierwszy_sektor_root=FAT->boot.zarezerwowane_sektory+FAT->boot.FAT_ile*FAT->boot.FAT_rozmiar+root_sektory-root_sektory;

    FAT->root_pierwszy_sektor=pierwszy_sektor_root;
    FAT->root_ile_sektorow=root_sektory;

    FAT->root=malloc(FAT->root_ile_sektorow*FAT->boot.bitow_na_sektor);
    if(FAT->root==NULL){
        errno = ENOMEM;
        free(FAT->FAT2);
        free(FAT->FAT1);
        free(FAT);
        return NULL;
    }

    if(disk_read(pdisk, pierwszy_sektor_root, FAT->root, root_sektory)==-1){
        errno=EINVAL;
        free(FAT->root);
        free(FAT->FAT2);
        free(FAT->FAT1);
        free(FAT);
        return NULL;
    }

    return FAT;
}

int fat_close(struct volume_t* wolumin){
    if (wolumin==NULL) {
        errno=EFAULT;
        return -1;
    }

    free(wolumin->root);
    free(wolumin->FAT2);
    free(wolumin->FAT1);
    free(wolumin);

    return 0;
}

uint16_t read_fat12_entry(const void *buffer, size_t size, uint16_t cluster){
    size_t byte_offset=cluster*3/2;
    if (byte_offset+1>=size)
        return 0xFFF;

    uint16_t entry;
    const uint8_t *byte_buffer=buffer;

    if (cluster%2==0)
        entry=(byte_buffer[byte_offset]|byte_buffer[byte_offset+1]<<8)&0x0FFF;
    else
        entry=(byte_buffer[byte_offset]>>4|byte_buffer[byte_offset+1]<<4)&0x0FFF;

    return entry;
}

struct lancuch_klastrow_t *get_chain_fat12(const void * const buffer, size_t size, uint16_t first_cluster){
    if (buffer==NULL||size<3)
        return NULL;

    struct lancuch_klastrow_t *lancuch=malloc(sizeof(struct lancuch_klastrow_t));
    if (lancuch==NULL)
        return NULL;

    lancuch->klastry=NULL;
    lancuch->rozmiar=0;

    while (first_cluster<0xFF8) {
        uint16_t *nowy_klaster=realloc(lancuch->klastry,(lancuch->rozmiar+1)*sizeof(uint16_t));
        if (nowy_klaster==NULL) {
            free(lancuch->klastry);
            free(lancuch);
            return NULL;
        }
        lancuch->klastry=nowy_klaster;
        lancuch->klastry[lancuch->rozmiar++]=first_cluster;
        first_cluster=read_fat12_entry(buffer, size, first_cluster);

        if (first_cluster==0||first_cluster>=0xFF8)
            break;
    }

    if (lancuch->rozmiar==0) {
        free(lancuch);
        return NULL;
    }

    return lancuch;
}

struct file_t* file_open(struct volume_t* wolumin, const char* file_name){
    if(wolumin==NULL||file_name==NULL){
        errno=EFAULT;
        return NULL;
    }

    struct file_t *plik=calloc(1,sizeof(struct file_t));
    if(plik==NULL){
        errno=ENOMEM;
        return NULL;
    }

    plik->wolumin=wolumin;

    struct dir_t *katalog=dir_open(wolumin, "\\");
    plik->katalog_plikow=katalog;

    struct dir_entry_t temp;
    while(dir_read(katalog, &temp)==0){
        plik->wejscie_pliku=temp;
        int i=0,flaga=1;

        while(file_name[i]!='\0' && plik->wejscie_pliku.name[i]!='.'){
            if(file_name[i]!=plik->wejscie_pliku.name[i]) {
                flaga=0;
                break;
            }
            i++;
        }

        if (flaga==1) {
            if(plik->wejscie_pliku.is_directory==1){
                errno=EISDIR;
                free(katalog);
                free(plik);
                return NULL;
            }

            plik->klastry_pliku=get_chain_fat12(wolumin->FAT1,wolumin->boot.bitow_na_sektor*wolumin->boot.FAT_rozmiar,plik->wejscie_pliku.first_cluster);
            return plik;
        }
    }

    errno=ENOENT;
    dir_close(katalog);
    free(plik);

    return NULL;
}

int file_close(struct file_t* stream){
    if (stream==NULL){
        errno=EFAULT;
        return -1;
    }

    free(stream->klastry_pliku->klastry);
    free(stream->klastry_pliku);
    dir_close(stream->katalog_plikow);
    free(stream);

    return 0;
}

size_t file_read(void *ptr, size_t size, size_t nmemb, struct file_t *stream){
    if(stream==NULL||ptr==NULL){
        errno=EFAULT;
        return -1;
    }

    size_t ile,ile_do_przeczytania;
    if(size*nmemb>stream->wejscie_pliku.size-stream->offset){
        ile=(stream->wejscie_pliku.size-stream->offset)/size;
        ile_do_przeczytania=stream->wejscie_pliku.size-stream->offset;
    }
    else{
        ile=nmemb;
        ile_do_przeczytania=size*nmemb;
    }

    void *buffer=malloc(stream->wolumin->boot.bitow_na_sektor*stream->wolumin->boot.sektory_na_klaster);
    if (buffer==NULL) {
        errno=ENOMEM;
        return -1;
    }

    uint32_t lacznie_odczytane_bajty=0, odczytane_bajty;
    for(int i=0;i<(int)stream->klastry_pliku->rozmiar;i++){
        if(!ile_do_przeczytania)
            break;

        if(disk_read(stream->wolumin->dysk,(stream->klastry_pliku->klastry[stream->offset/(stream->wolumin->boot.bitow_na_sektor*stream->wolumin->boot.sektory_na_klaster)]-2)*stream->wolumin->boot.sektory_na_klaster+stream->wolumin->boot.zarezerwowane_sektory+stream->wolumin->boot.FAT_ile*stream->wolumin->boot.FAT_rozmiar+stream->wolumin->root_ile_sektorow, buffer,stream->wolumin->boot.sektory_na_klaster)==-1){
            errno=ERANGE;
            free(buffer);
            return -1;
        }

        uint32_t offset=stream->offset%(stream->wolumin->boot.bitow_na_sektor*stream->wolumin->boot.sektory_na_klaster);
        uint32_t pozostale_bajty=stream->wolumin->boot.bitow_na_sektor*stream->wolumin->boot.sektory_na_klaster-offset;

        if(ile_do_przeczytania>=stream->wolumin->boot.bitow_na_sektor*stream->wolumin->boot.sektory_na_klaster){
            memcpy((char *)ptr+lacznie_odczytane_bajty,(char *)buffer+offset,pozostale_bajty);
            lacznie_odczytane_bajty=lacznie_odczytane_bajty+pozostale_bajty;
            ile_do_przeczytania=ile_do_przeczytania-pozostale_bajty;
            odczytane_bajty=pozostale_bajty;
        }
        else{
            if(ile_do_przeczytania > pozostale_bajty){
                memcpy((char *)ptr+lacznie_odczytane_bajty,(char *)buffer+offset,pozostale_bajty);
                lacznie_odczytane_bajty=lacznie_odczytane_bajty+pozostale_bajty;
                ile_do_przeczytania=ile_do_przeczytania-pozostale_bajty;
                odczytane_bajty=pozostale_bajty;
            }
            else{
                memcpy((char *)ptr+lacznie_odczytane_bajty,(char *)buffer+offset,ile_do_przeczytania);
                lacznie_odczytane_bajty=lacznie_odczytane_bajty+ile_do_przeczytania;
                odczytane_bajty=ile_do_przeczytania;
                ile_do_przeczytania=0;
            }
        }

        file_seek(stream, (int32_t)odczytane_bajty, SEEK_CUR);
    }

    free(buffer);

    return ile;
}

int32_t file_seek(struct file_t* stream, int32_t offset, int whence){
    if(stream==NULL){
        errno=EFAULT;
        return -1;
    }

    if(whence!=0 && whence!=1 && whence!=2){
        errno=EINVAL;
        return -1;
    }

    if(whence==0){
        if(offset>(int)stream->wejscie_pliku.size || offset<0){
            errno=ENXIO;
            return -1;
        }
        stream->offset=offset;
    }
    else if(whence == 1){
        if(stream->offset+offset>(int)stream->wejscie_pliku.size || stream->offset+offset<0){
            errno=ENXIO;
            return -1;
        }
        stream->offset=stream->offset+offset;
    }
    else{
        if(stream->wejscie_pliku.size+offset>stream->wejscie_pliku.size){
            errno=ENXIO;
            return -1;
        }
        stream->offset=stream->wejscie_pliku.size+offset;
    }
    return stream->offset;
}

struct dir_t* dir_open(struct volume_t* wolumin, const char* dir_path){
    if(wolumin==NULL||dir_path==NULL){
        errno=EFAULT;
        return NULL;
    }

    if(strcmp(dir_path, "\\")!=0){
        if(strcmp(dir_path, "/")!=0){
            errno=ENOENT;
            return NULL;
        }
    }

    struct dir_t *katalog=calloc(1, sizeof(struct dir_t));
    if(katalog==NULL){
        errno=ENOMEM;
        return NULL;
    }

    katalog->wolumin=wolumin;

    return katalog;
}

int dir_read(struct dir_t* pdir, struct dir_entry_t* pentry){
    if(pdir==NULL||pentry==NULL){
        errno=EFAULT;
        return -1;
    }

    memset(pentry,0,sizeof(struct dir_entry_t));
    struct dir_entry_pelny_t *wejscie=(struct dir_entry_pelny_t *)pdir->wolumin->root+pdir->ile_wczytano;
    if(wejscie->nazwa[0]==0x00)
        return 1;
    if((uint8_t )wejscie->nazwa[0]==0xe5){
        pdir->ile_wczytano++;
        return dir_read(pdir, pentry);
    }

    pentry->size=wejscie->rozmiar_pliku;
    pentry->is_archived=wejscie->bity.bit_5;
    pentry->is_readonly=wejscie->bity.bit_0;
    pentry->is_system=wejscie->bity.bit_2;
    pentry->is_hidden=wejscie->bity.bit_1;
    pentry->is_directory=wejscie->bity.bit_4;
    pentry->first_cluster=wejscie->ostatni_klaster;

    if (pentry->is_directory) {
        for (int i=0;i<11;i++)
            if (wejscie->nazwa[i]!=' ')
                pentry->name[i]=wejscie->nazwa[i];
    }
    else{
        int i=0;
        while (i<8){
            if (wejscie->nazwa[i]!=' ')
                pentry->name[i]=wejscie->nazwa[i];
            else
                break;
            i++;
        }
        if (wejscie->nazwa[8]!=' ') {
            pentry->name[i++]='.';
            for (int j=8;j<11;j++)
                if (wejscie->nazwa[j]!=' ')
                    pentry->name[i++]=wejscie->nazwa[j];
        }
    }

    pdir->ile_wczytano++;

    return 0;
}

int dir_close(struct dir_t* pdir){
    if (pdir==NULL) {
        errno=EFAULT;
        return -1;
    }

    free(pdir);
    return 0;
}

