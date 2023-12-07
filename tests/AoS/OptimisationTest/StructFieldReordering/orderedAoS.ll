; ModuleID = 'orderedAoS.c'
source_filename = "orderedAoS.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.nodeOneNew = type { double, i32, i8, i8 }

@.str = private unnamed_addr constant [16 x i8] c"Validity check\0A\00", align 1
@.str.1 = private unnamed_addr constant [11 x i8] c"a: %d\0A---\0A\00", align 1
@.str.2 = private unnamed_addr constant [11 x i8] c"b: %f\0A---\0A\00", align 1
@.str.3 = private unnamed_addr constant [11 x i8] c"c: %c\0A---\0A\00", align 1
@.str.4 = private unnamed_addr constant [11 x i8] c"d: %c\0A---\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNodeOneNew(ptr noundef %array, i32 noundef %size) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %size.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load ptr, ptr %array.addr, align 8
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds %struct.nodeOneNew, ptr %2, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOneNew, ptr %arrayidx, i32 0, i32 1
  store i32 17, ptr %a, align 8
  %4 = load ptr, ptr %array.addr, align 8
  %5 = load i32, ptr %i, align 4
  %idxprom1 = sext i32 %5 to i64
  %arrayidx2 = getelementptr inbounds %struct.nodeOneNew, ptr %4, i64 %idxprom1
  %b = getelementptr inbounds %struct.nodeOneNew, ptr %arrayidx2, i32 0, i32 0
  store double 1.000000e+01, ptr %b, align 8
  %6 = load ptr, ptr %array.addr, align 8
  %7 = load i32, ptr %i, align 4
  %idxprom3 = sext i32 %7 to i64
  %arrayidx4 = getelementptr inbounds %struct.nodeOneNew, ptr %6, i64 %idxprom3
  %c = getelementptr inbounds %struct.nodeOneNew, ptr %arrayidx4, i32 0, i32 2
  store i8 99, ptr %c, align 4
  %8 = load ptr, ptr %array.addr, align 8
  %9 = load i32, ptr %i, align 4
  %idxprom5 = sext i32 %9 to i64
  %arrayidx6 = getelementptr inbounds %struct.nodeOneNew, ptr %8, i64 %idxprom5
  %d = getelementptr inbounds %struct.nodeOneNew, ptr %arrayidx6, i32 0, i32 3
  store i8 100, ptr %d, align 1
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %10 = load i32, ptr %i, align 4
  %inc = add nsw i32 %10, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !6

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @multNodeOneNew(ptr noundef %array, i32 noundef %size) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %size.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load ptr, ptr %array.addr, align 8
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds %struct.nodeOneNew, ptr %2, i64 %idxprom
  %b = getelementptr inbounds %struct.nodeOneNew, ptr %arrayidx, i32 0, i32 0
  %4 = load double, ptr %b, align 8
  %5 = load ptr, ptr %array.addr, align 8
  %6 = load i32, ptr %i, align 4
  %idxprom1 = sext i32 %6 to i64
  %arrayidx2 = getelementptr inbounds %struct.nodeOneNew, ptr %5, i64 %idxprom1
  %a = getelementptr inbounds %struct.nodeOneNew, ptr %arrayidx2, i32 0, i32 1
  %7 = load i32, ptr %a, align 8
  %conv = sitofp i32 %7 to double
  %mul = fmul double %4, %conv
  %div = fdiv double %mul, 2.000000e+01
  %conv3 = fptosi double %div to i32
  %8 = load ptr, ptr %array.addr, align 8
  %9 = load i32, ptr %i, align 4
  %idxprom4 = sext i32 %9 to i64
  %arrayidx5 = getelementptr inbounds %struct.nodeOneNew, ptr %8, i64 %idxprom4
  %a6 = getelementptr inbounds %struct.nodeOneNew, ptr %arrayidx5, i32 0, i32 1
  store i32 %conv3, ptr %a6, align 8
  %10 = load ptr, ptr %array.addr, align 8
  %11 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %11 to i64
  %arrayidx8 = getelementptr inbounds %struct.nodeOneNew, ptr %10, i64 %idxprom7
  %a9 = getelementptr inbounds %struct.nodeOneNew, ptr %arrayidx8, i32 0, i32 1
  %12 = load i32, ptr %a9, align 8
  %conv10 = sitofp i32 %12 to double
  %13 = load ptr, ptr %array.addr, align 8
  %14 = load i32, ptr %i, align 4
  %idxprom11 = sext i32 %14 to i64
  %arrayidx12 = getelementptr inbounds %struct.nodeOneNew, ptr %13, i64 %idxprom11
  %b13 = getelementptr inbounds %struct.nodeOneNew, ptr %arrayidx12, i32 0, i32 0
  %15 = load double, ptr %b13, align 8
  %sub = fsub double %conv10, %15
  %mul14 = fmul double %sub, 2.000000e+01
  %16 = load ptr, ptr %array.addr, align 8
  %17 = load i32, ptr %i, align 4
  %idxprom15 = sext i32 %17 to i64
  %arrayidx16 = getelementptr inbounds %struct.nodeOneNew, ptr %16, i64 %idxprom15
  %b17 = getelementptr inbounds %struct.nodeOneNew, ptr %arrayidx16, i32 0, i32 0
  store double %mul14, ptr %b17, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %18 = load i32, ptr %i, align 4
  %inc = add nsw i32 %18, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !8

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @multArrays(ptr noundef %arrayOne, ptr noundef %arrayTwo, i32 noundef %size) #0 {
entry:
  %arrayOne.addr = alloca ptr, align 8
  %arrayTwo.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %j = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %arrayOne, ptr %arrayOne.addr, align 8
  store ptr %arrayTwo, ptr %arrayTwo.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %j, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc19, %entry
  %0 = load i32, ptr %j, align 4
  %cmp = icmp slt i32 %0, 70
  br i1 %cmp, label %for.body, label %for.end21

for.body:                                         ; preds = %for.cond
  store i32 0, ptr %i, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %1 = load i32, ptr %i, align 4
  %2 = load i32, ptr %size.addr, align 4
  %cmp2 = icmp slt i32 %1, %2
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %3 = load ptr, ptr %arrayOne.addr, align 8
  %4 = load i32, ptr %i, align 4
  %idxprom = sext i32 %4 to i64
  %arrayidx = getelementptr inbounds %struct.nodeOneNew, ptr %3, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOneNew, ptr %arrayidx, i32 0, i32 1
  %5 = load i32, ptr %a, align 8
  %6 = load ptr, ptr %arrayTwo.addr, align 8
  %7 = load i32, ptr %i, align 4
  %idxprom4 = sext i32 %7 to i64
  %arrayidx5 = getelementptr inbounds %struct.nodeOneNew, ptr %6, i64 %idxprom4
  %a6 = getelementptr inbounds %struct.nodeOneNew, ptr %arrayidx5, i32 0, i32 1
  %8 = load i32, ptr %a6, align 8
  %mul = mul nsw i32 %5, %8
  %9 = load ptr, ptr %arrayOne.addr, align 8
  %10 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %10 to i64
  %arrayidx8 = getelementptr inbounds %struct.nodeOneNew, ptr %9, i64 %idxprom7
  %a9 = getelementptr inbounds %struct.nodeOneNew, ptr %arrayidx8, i32 0, i32 1
  store i32 %mul, ptr %a9, align 8
  %11 = load ptr, ptr %arrayOne.addr, align 8
  %12 = load i32, ptr %i, align 4
  %idxprom10 = sext i32 %12 to i64
  %arrayidx11 = getelementptr inbounds %struct.nodeOneNew, ptr %11, i64 %idxprom10
  %b = getelementptr inbounds %struct.nodeOneNew, ptr %arrayidx11, i32 0, i32 0
  %13 = load double, ptr %b, align 8
  %14 = load ptr, ptr %arrayTwo.addr, align 8
  %15 = load i32, ptr %i, align 4
  %idxprom12 = sext i32 %15 to i64
  %arrayidx13 = getelementptr inbounds %struct.nodeOneNew, ptr %14, i64 %idxprom12
  %b14 = getelementptr inbounds %struct.nodeOneNew, ptr %arrayidx13, i32 0, i32 0
  %16 = load double, ptr %b14, align 8
  %mul15 = fmul double %13, %16
  %17 = load ptr, ptr %arrayOne.addr, align 8
  %18 = load i32, ptr %i, align 4
  %idxprom16 = sext i32 %18 to i64
  %arrayidx17 = getelementptr inbounds %struct.nodeOneNew, ptr %17, i64 %idxprom16
  %b18 = getelementptr inbounds %struct.nodeOneNew, ptr %arrayidx17, i32 0, i32 0
  store double %mul15, ptr %b18, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %19 = load i32, ptr %i, align 4
  %inc = add nsw i32 %19, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond1, !llvm.loop !9

for.end:                                          ; preds = %for.cond1
  br label %for.inc19

for.inc19:                                        ; preds = %for.end
  %20 = load i32, ptr %j, align 4
  %inc20 = add nsw i32 %20, 1
  store i32 %inc20, ptr %j, align 4
  br label %for.cond, !llvm.loop !10

for.end21:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %n = alloca i32, align 4
  %i = alloca i32, align 4
  %arrayOneNew = alloca ptr, align 8
  %arrayTwoNew = alloca ptr, align 8
  store i32 0, ptr %retval, align 4
  store i32 999999, ptr %n, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %cmp = icmp slt i32 %0, 100
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %1 = load i32, ptr %n, align 4
  %conv = sext i32 %1 to i64
  %mul = mul i64 %conv, 16
  %call = call noalias ptr @malloc(i64 noundef %mul) #4
  store ptr %call, ptr %arrayOneNew, align 8
  %2 = load i32, ptr %n, align 4
  %conv1 = sext i32 %2 to i64
  %mul2 = mul i64 %conv1, 16
  %call3 = call noalias ptr @malloc(i64 noundef %mul2) #4
  store ptr %call3, ptr %arrayTwoNew, align 8
  %3 = load ptr, ptr %arrayOneNew, align 8
  %4 = load i32, ptr %n, align 4
  call void @populateNodeOneNew(ptr noundef %3, i32 noundef %4)
  %5 = load ptr, ptr %arrayTwoNew, align 8
  %6 = load i32, ptr %n, align 4
  call void @populateNodeOneNew(ptr noundef %5, i32 noundef %6)
  %7 = load ptr, ptr %arrayOneNew, align 8
  %8 = load i32, ptr %n, align 4
  call void @multNodeOneNew(ptr noundef %7, i32 noundef %8)
  %9 = load ptr, ptr %arrayOneNew, align 8
  %10 = load ptr, ptr %arrayTwoNew, align 8
  %11 = load i32, ptr %n, align 4
  call void @multArrays(ptr noundef %9, ptr noundef %10, i32 noundef %11)
  %12 = load i32, ptr %i, align 4
  %cmp4 = icmp eq i32 %12, 79
  br i1 %cmp4, label %if.then, label %if.end

if.then:                                          ; preds = %for.body
  %call6 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  %13 = load ptr, ptr %arrayOneNew, align 8
  %arrayidx = getelementptr inbounds %struct.nodeOneNew, ptr %13, i64 1
  %a = getelementptr inbounds %struct.nodeOneNew, ptr %arrayidx, i32 0, i32 1
  %14 = load i32, ptr %a, align 8
  %call7 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %14)
  %15 = load ptr, ptr %arrayOneNew, align 8
  %arrayidx8 = getelementptr inbounds %struct.nodeOneNew, ptr %15, i64 1
  %b = getelementptr inbounds %struct.nodeOneNew, ptr %arrayidx8, i32 0, i32 0
  %16 = load double, ptr %b, align 8
  %call9 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, double noundef %16)
  %17 = load ptr, ptr %arrayOneNew, align 8
  %arrayidx10 = getelementptr inbounds %struct.nodeOneNew, ptr %17, i64 1
  %c = getelementptr inbounds %struct.nodeOneNew, ptr %arrayidx10, i32 0, i32 2
  %18 = load i8, ptr %c, align 4
  %conv11 = sext i8 %18 to i32
  %call12 = call i32 (ptr, ...) @printf(ptr noundef @.str.3, i32 noundef %conv11)
  %19 = load ptr, ptr %arrayOneNew, align 8
  %arrayidx13 = getelementptr inbounds %struct.nodeOneNew, ptr %19, i64 1
  %d = getelementptr inbounds %struct.nodeOneNew, ptr %arrayidx13, i32 0, i32 3
  %20 = load i8, ptr %d, align 1
  %conv14 = sext i8 %20 to i32
  %call15 = call i32 (ptr, ...) @printf(ptr noundef @.str.4, i32 noundef %conv14)
  br label %if.end

if.end:                                           ; preds = %if.then, %for.body
  %21 = load ptr, ptr %arrayOneNew, align 8
  call void @free(ptr noundef %21) #5
  %22 = load ptr, ptr %arrayTwoNew, align 8
  call void @free(ptr noundef %22) #5
  br label %for.inc

for.inc:                                          ; preds = %if.end
  %23 = load i32, ptr %i, align 4
  %inc = add nsw i32 %23, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !11

for.end:                                          ; preds = %for.cond
  ret i32 0
}

; Function Attrs: nounwind allocsize(0)
declare noalias ptr @malloc(i64 noundef) #1

declare i32 @printf(ptr noundef, ...) #2

; Function Attrs: nounwind
declare void @free(ptr noundef) #3

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind allocsize(0) }
attributes #5 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 18.0.0 (https://github.com/llvm/llvm-project.git 893416051d517d979481510fe3b52d83ba0d1e01)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
!11 = distinct !{!11, !7}
